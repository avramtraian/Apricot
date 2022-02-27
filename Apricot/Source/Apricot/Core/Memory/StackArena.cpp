// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "StackArena.h"

namespace Apricot {

	namespace Utils {

		static AStackArena::APage* ConstructNewPage(uint8* ArenaMemory, uint64& Offset, uint64 PageSizeBytes)
		{
			Offset += GetAlignmentOffset(Offset, sizeof(void*));

			AStackArena::APage* NewPage = (AStackArena::APage*)(ArenaMemory + Offset);
			MemConstruct<AStackArena::APage>(NewPage);
			Offset += sizeof(AStackArena::APage);

			NewPage->MemoryBlock = ArenaMemory + Offset;
			NewPage->SizeBytes = PageSizeBytes;
			NewPage->AllocatedBytes = 0;

			Offset += NewPage->SizeBytes;

			return NewPage;
		}

	}

	NODISCARD TSharedPtr<AStackArena> AStackArena::Create(const AStackArenaSpecification& Specification)
	{
		return MakeShared<AStackArena>(Specification);
	}

	NODISCARD uint64 AStackArena::GetPageMemoryRequirement(uint64 PageSize)
	{
		uint64 MemoryRequirement = 0;

		MemoryRequirement += sizeof(APage);
		MemoryRequirement += PageSize;

		return MemoryRequirement;
	}

	NODISCARD uint64 AStackArena::GetMemoryRequirement(const AStackArenaSpecification& Specification)
	{
		uint64 MemoryRequirement = 0;

		for (uint64 Index = 0; Index < Specification.PagesCount; Index++)
		{
			MemoryRequirement += GetAlignmentOffset(MemoryRequirement, sizeof(void*));
			MemoryRequirement += GetPageMemoryRequirement(Specification.PageSizes[Index]);
		}

		return MemoryRequirement;
	}

	AStackArena::AStackArena(const AStackArenaSpecification& Specification)
		: m_Specification(Specification), m_CurrentPage(0)
	{
		m_Pages.SetCapacity(m_Specification.PagesCount);

		uint8* ArenaMemory = (uint8*)m_Specification.ArenaMemory;

		if (m_Specification.bBulkAllocateSpecPages || ArenaMemory)
		{
			if (!ArenaMemory)
			{
				ArenaMemory = (uint8*)GMalloc->Alloc(GetMemoryRequirement(m_Specification));
			}
			uint64 MemoryOffset = 0;

			for (uint64 Index = 0; Index < m_Specification.PagesCount; Index++)
			{
				m_Pages.PushBack(Utils::ConstructNewPage(ArenaMemory, MemoryOffset, m_Specification.PageSizes[Index]));
			}
		}
		else
		{
			for (uint64 Index = 0; Index < m_Specification.PagesCount; Index++)
			{
				uint64 MemoryOffset = 0;
				uint64 PageSizeBytes = m_Specification.PageSizes[Index];

				ArenaMemory = (uint8*)GMalloc->Alloc(GetPageMemoryRequirement(PageSizeBytes));
				m_Pages.PushBack(Utils::ConstructNewPage(ArenaMemory, MemoryOffset, PageSizeBytes));
			}
		}
	}

	AStackArena::~AStackArena()
	{
		if (!m_Specification.bUseArenaMemoryAlways)
		{
			for (uint64 Index = m_Specification.PagesCount; Index < m_Pages.Size(); Index++)
			{
				GMalloc->Free(m_Pages[Index], GetPageMemoryRequirement(m_Pages[Index]->SizeBytes));
			}
		}
		if (!m_Specification.ArenaMemory && !m_Pages.IsEmpty())
		{
			GMalloc->Free(m_Pages[0], GetMemoryRequirement(m_Specification));
		}
	}

	uint64 AStackArena::GetTotalSize() const
	{
		uint64 TotalSizeBytes = 0;
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			TotalSizeBytes += m_Pages[Index]->SizeBytes;
		}
		return TotalSizeBytes;
	}

	uint64 AStackArena::GetAllocatedSize() const
	{
		uint64 AllocatedBytes = 0;
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			AllocatedBytes += m_Pages[Index]->AllocatedBytes;
		}
		return AllocatedBytes;
	}

	uint64 AStackArena::GetFreeSize() const
	{
		uint64 FreeSizeBytes = 0;
		for (uint64 Index = m_CurrentPage; Index < m_Pages.Size(); Index++)
		{
			FreeSizeBytes += (m_Pages[Index]->SizeBytes - m_Pages[Index]->AllocatedBytes);
		}
		return FreeSizeBytes;
	}

	const TChar* AStackArena::GetDebugName() const
	{
		return TEXT("STACK_ARENA");
	}

	void* AStackArena::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (m_Pages.Size() == 0)
		{
			GCrashReporter->MemoryState.Arena = this;
			GCrashReporter->SubmitArenaFailure(ACrashReporter::EMemoryOperation::Allocate, EMemoryError::InvalidArena);
			return nullptr;
		}
		if (Size == 0)
		{
			GCrashReporter->MemoryState.Arena = this;
			GCrashReporter->MemoryState.Size = Size;
			GCrashReporter->SubmitArenaFailure(ACrashReporter::EMemoryOperation::Allocate, EMemoryError::InvalidSize);
			return nullptr;
		}
		if (Alignment == 0)
		{
			GCrashReporter->MemoryState.Arena = this;
			GCrashReporter->MemoryState.Alignment = Alignment;
			GCrashReporter->SubmitArenaFailure(ACrashReporter::EMemoryOperation::Allocate, EMemoryError::InvalidAlignment);
			return nullptr;
		}

		if (m_Specification.bAllowAlignment)
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				uint64 AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
				if (Page->AllocatedBytes + AlignmentOffset + Size + sizeof(AAlignmentInfo) <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes + AlignmentOffset;
					AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
					AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
					Page->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
					return Memory;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			uint64 AlignmentOffset = GetAlignmentOffset((uint8*)NewPage->MemoryBlock, Alignment);

			void* Memory = (uint8*)NewPage->MemoryBlock + AlignmentOffset;
			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
			AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
			NewPage->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
			return Memory;
		}
		else
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				if (Page->AllocatedBytes + Size <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes;
					Page->AllocatedBytes += Size;
					return Memory;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			void* Memory = (uint8*)NewPage->MemoryBlock;
			NewPage->AllocatedBytes += Size;
			return Memory;
		}
	}

	int32 AStackArena::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (OutPointer == nullptr)
		{
			return (int32)EMemoryError::InvalidOuterPointer;
		}
		if (m_Pages.Size() == 0)
		{
			*OutPointer = nullptr;
			return (int32)EMemoryError::InvalidArena;
		}
		if (Size == 0)
		{
			*OutPointer = nullptr;
			return (int32)EMemoryError::InvalidSize;
		}
		if (Alignment == 0)
		{
			*OutPointer = nullptr;
			return (int32)EMemoryError::InvalidAlignment;
		}

		if (m_Specification.bAllowAlignment)
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				uint64 AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
				if (Page->AllocatedBytes + AlignmentOffset + Size + sizeof(AAlignmentInfo) <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes + AlignmentOffset;
					AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
					AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
					Page->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
					*OutPointer = Memory;
					return (int32)EMemoryError::Success;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			uint64 AlignmentOffset = GetAlignmentOffset((uint8*)NewPage->MemoryBlock, Alignment);

			void* Memory = (uint8*)NewPage->MemoryBlock + AlignmentOffset;
			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
			AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
			NewPage->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
			*OutPointer = Memory;
			return (int32)EMemoryError::Success;
		}
		else
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				if (Page->AllocatedBytes + Size <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes;
					Page->AllocatedBytes += Size;
					*OutPointer = Memory;
					return (int32)EMemoryError::Success;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			void* Memory = (uint8*)NewPage->MemoryBlock;
			NewPage->AllocatedBytes += Size;
			*OutPointer = Memory;
			return (int32)EMemoryError::Success;
		}
	}

	void* AStackArena::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (m_Specification.bAllowAlignment)
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				uint64 AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
				if (Page->AllocatedBytes + AlignmentOffset + Size + sizeof(AAlignmentInfo) <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes + AlignmentOffset;
					AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
					AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
					Page->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
					return Memory;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			uint64 AlignmentOffset = GetAlignmentOffset((uint8*)NewPage->MemoryBlock, Alignment);

			void* Memory = (uint8*)NewPage->MemoryBlock + AlignmentOffset;
			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Memory + Size);
			AlignmentInfo->AlignmentOffset = (uint16)AlignmentOffset;
			NewPage->AllocatedBytes += (AlignmentOffset + Size + sizeof(AAlignmentInfo));
			return Memory;
		}
		else
		{
			uint64 CurrentPage = m_CurrentPage;
			for (uint64 Index = CurrentPage; Index < m_Pages.Size(); Index++)
			{
				m_CurrentPage = Index;
				APage* Page = m_Pages[Index];

				if (Page->AllocatedBytes + Size <= Page->SizeBytes)
				{
					void* Memory = (uint8*)Page->MemoryBlock + Page->AllocatedBytes;
					Page->AllocatedBytes += Size;
					return Memory;
				}
			}

			APage* NewPage = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			void* Memory = (uint8*)NewPage->MemoryBlock;
			NewPage->AllocatedBytes += Size;
			return Memory;
		}
	}

	void AStackArena::Free(void* Allocation, uint64 Size)
	{
		uint64 PopSize = Size;
		if (m_Specification.bAllowAlignment)
		{
			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Allocation + Size);
			PopSize += AlignmentInfo->AlignmentOffset;
		}

		bool8 bFoundHomePage = false;

		for (int64 Index = m_CurrentPage; Index >= 0; Index--)
		{
			if (!IsAddressBetween(Allocation, m_Pages[Index]->MemoryBlock, (uint8*)m_Pages[Index]->MemoryBlock + m_Pages[Index]->SizeBytes))
			{
				PopSize += m_Pages[Index]->AllocatedBytes;
			}
			else
			{
				bFoundHomePage = true;
				break;
			}
		}

		if (!bFoundHomePage)
		{

		}

		Pop(PopSize);
	}

	int32 AStackArena::TryFree(void* Allocation, uint64 Size)
	{
		if (m_Pages.Size() == 0)
		{
			return (int32)EMemoryError::InvalidArena;
		}

		if (Allocation == nullptr)
		{
			return (int32)EMemoryError::InvalidMemoryPtr;
		}
		if (Size == 0)
		{
			return (int32)EMemoryError::InvalidSize;
		}

		APage* Page = m_Pages[m_CurrentPage];
		if (Page->AllocatedBytes == 0)
		{
			Page = m_Pages[--m_CurrentPage];
		}

		if (m_Specification.bAllowAlignment)
		{
			if ((uintptr)Allocation + Size + sizeof(AAlignmentInfo) != (uintptr)Page->MemoryBlock + Page->AllocatedBytes)
			{
				return (int32)EMemoryError::PointerOutOfRange;
			}

			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Allocation + Size);
			return TryPop(AlignmentInfo->AlignmentOffset + Size + sizeof(AAlignmentInfo));
		}
		else
		{
			if ((uintptr)Allocation + Size != (uintptr)Page->MemoryBlock + Page->AllocatedBytes)
			{
				return (int32)EMemoryError::PointerOutOfRange;
			}

			return TryPop(Size);
		}
		return (int32)EMemoryError::Success;
	}

	void AStackArena::FreeUnsafe(void* Allocation, uint64 Size)
	{
		APage* Page = m_Pages[m_CurrentPage];
		if (Page->AllocatedBytes == 0)
		{
			Page = m_Pages[--m_CurrentPage];
		}

		if (m_Specification.bAllowAlignment)
		{
			AAlignmentInfo* AlignmentInfo = (AAlignmentInfo*)((uint8*)Allocation + Size);
			return PopUnsafe(AlignmentInfo->AlignmentOffset + Size + sizeof(AAlignmentInfo));
		}
		else
		{
			return PopUnsafe(Size);
		}
	}

	void AStackArena::FreeAll()
	{
		for (uint64 Index = 0; Index < m_CurrentPage + 1; Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
	}

	int32 AStackArena::TryFreeAll()
	{
		for (uint64 Index = 0; Index < m_CurrentPage + 1; Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
		return (int32)EMemoryError::Success;
	}

	void AStackArena::FreeAllUnsafe()
	{
		for (uint64 Index = 0; Index < m_CurrentPage + 1; Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
	}

	void AStackArena::GarbageCollect()
	{
		for (int64 Index = m_Pages.Size() - 1; Index >= (int64)m_CurrentPage; Index--)
		{
			if (m_Pages[Index]->AllocatedBytes > 0)
			{
				break;
			}

			m_Pages.PopBack();
		}
		m_CurrentPage = m_Pages.Size() - 1;
	}

	void AStackArena::Pop(uint64 Size)
	{
		uint64 PopSize = Size;

		for (int64 Index = m_CurrentPage; Index >= 0; Index--)
		{
			APage* Page = m_Pages[Index];
			m_CurrentPage = Index;

			if (PopSize > Page->AllocatedBytes)
			{
				PopSize -= Page->AllocatedBytes;
				Page->AllocatedBytes = 0;
			}
			else
			{
				Page->AllocatedBytes -= PopSize;
				PopSize = 0;
				break;
			}
		}
	}

	int32 AStackArena::TryPop(uint64 Size)
	{
		uint64 PopSize = Size;

		for (int64 Index = m_CurrentPage; Index >= 0; Index--)
		{
			m_CurrentPage = Index;
			APage* Page = m_Pages[Index];

			if (PopSize > Page->AllocatedBytes)
			{
				PopSize -= Page->AllocatedBytes;
				Page->AllocatedBytes = 0;
			}
			else
			{
				Page->AllocatedBytes -= PopSize;
				PopSize = 0;
				break;
			}
		}
		return (int32)EMemoryError::Success;
	}

	void AStackArena::PopUnsafe(uint64 Size)
	{
		uint64 PopSize = Size;

		for (int64 Index = m_CurrentPage; Index >= 0; Index--)
		{
			m_CurrentPage = Index;
			APage* Page = m_Pages[Index];

			if (PopSize > Page->AllocatedBytes)
			{
				PopSize -= Page->AllocatedBytes;
				Page->AllocatedBytes = 0;
			}
			else
			{
				Page->AllocatedBytes -= PopSize;
				PopSize = 0;
				break;
			}
		}
	}

	AStackArena::APage* AStackArena::AllocateNewPage(uint64 PageSize)
	{
		APage* NewPage = (APage*)GMalloc->Alloc(sizeof(APage) + PageSize);
		m_Pages.PushBack(NewPage);

		NewPage->MemoryBlock = (uint8*)NewPage + sizeof(APage);
		NewPage->SizeBytes = PageSize;
		NewPage->AllocatedBytes = 0;

		return NewPage;
	}

	uint64 AStackArena::GetOptimalPageSize(uint64 RequestedAllocationSize)
	{
		return m_Pages.Back()->SizeBytes > RequestedAllocationSize ? m_Pages.Back()->SizeBytes : (RequestedAllocationSize + RequestedAllocationSize / 2);
	}

	bool8 AStackArena::CouldBeFreed(void* Allocation, uint64 Size) const
	{
		return true;
	}

}