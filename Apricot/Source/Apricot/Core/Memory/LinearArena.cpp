// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "LinearArena.h"

namespace Apricot {

	namespace Utils {

		static ALinearArena::APage* ConstructNewPage(uint8* ArenaMemory, uint64& Offset, uint64 PageSize)
		{
			Offset += GetAlignmentOffset(Offset, sizeof(void*));

			ALinearArena::APage* NewPage = (ALinearArena::APage*)(ArenaMemory + Offset);
			MemConstruct<ALinearArena::APage>(NewPage);
			Offset += sizeof(ALinearArena::APage);

			NewPage->MemoryBlock = ArenaMemory + Offset;
			NewPage->SizeBytes = PageSize;
			NewPage->AllocatedBytes = 0;

			Offset += NewPage->SizeBytes;

			return NewPage;
		}

	}

	NODISCARD TSharedPtr<ALinearArena> ALinearArena::Create(const ALinearArenaSpecification& Specification)
	{
		return MakeShared<ALinearArena>(Specification);
	}

	NODISCARD uint64 ALinearArena::GetPageMemoryRequirement(uint64 PageSizeBytes)
	{
		uint64 MemoryRequirement = 0;

		MemoryRequirement += sizeof(APage);
		MemoryRequirement += PageSizeBytes;

		return MemoryRequirement;
	}

	NODISCARD uint64 ALinearArena::GetMemoryRequirement(const ALinearArenaSpecification& Specification)
	{
		uint64 MemoryRequirement = 0;

		for (uint64 Index = 0; Index < Specification.PagesCount; Index++)
		{
			MemoryRequirement += GetAlignmentOffset(MemoryRequirement, sizeof(void*));
			MemoryRequirement += GetPageMemoryRequirement(Specification.PageSizes[Index]);
		}

		return MemoryRequirement;
	}

	ALinearArena::ALinearArena(const ALinearArenaSpecification& Specification)
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

			if (ArenaMemory)
			{
				m_Specification.ArenaMemoryOffset = MemoryOffset;
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

	ALinearArena::~ALinearArena()
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

	uint64 ALinearArena::GetTotalSize() const
	{
		uint64 TotalSizeBytes = 0;
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			TotalSizeBytes += m_Pages[Index]->SizeBytes;
		}
		return TotalSizeBytes;
	}

	uint64 ALinearArena::GetAllocatedSize() const
	{
		uint64 AllocatedBytes = 0;
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			AllocatedBytes += m_Pages[Index]->AllocatedBytes;
		}
		return AllocatedBytes;
	}

	uint64 ALinearArena::GetFreeSize() const
	{
		uint64 FreeSizeBytes = 0;
		for (uint64 Index = m_CurrentPage; Index < m_Pages.Size(); Index++)
		{
			FreeSizeBytes += (m_Pages[Index]->SizeBytes - m_Pages[Index]->AllocatedBytes);
		}
		return FreeSizeBytes;
	}

	const TChar* ALinearArena::GetDebugName() const
	{
		return TEXT("LINEAR_ARENA");
	}

	NODISCARD void* ALinearArena::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		APage* Page = m_Pages[m_CurrentPage];
		uint64 AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);

		if (Page->AllocatedBytes + AlignmentOffset + Size > Page->SizeBytes)
		{
			for (uint64 Index = m_CurrentPage + 1; Index < m_Pages.Size(); Index++)
			{
				Page = m_Pages[Index];
				AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
				if (Page->AllocatedBytes + AlignmentOffset + Size <= Page->SizeBytes)
				{
					m_CurrentPage = Index;
					Page->AllocatedBytes += (AlignmentOffset + Size);
					return (uint8*)Page->MemoryBlock + AlignmentOffset;
				}
			}

			if (!m_Specification.bShouldGrow)
			{
				return nullptr;
			}

			Page = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
		}

		Page->AllocatedBytes += (AlignmentOffset + Size);
		return (uint8*)Page->MemoryBlock + AlignmentOffset;
	}

	NODISCARD int32 ALinearArena::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (OutPointer == nullptr)
		{
			return (int16)EMemoryError::InvalidOuterPointer;
		}
		if (m_Pages.IsEmpty())
		{
			*OutPointer = nullptr;
			return (int16)EMemoryError::InvalidArena;
		}
		if (Size == 0)
		{
			*OutPointer = nullptr;
			return (int16)EMemoryError::InvalidSize;
		}
		if (Alignment == 0)
		{
			*OutPointer = nullptr;
			return (int16)EMemoryError::InvalidAlignment;
		}

		APage* Page = m_Pages[m_CurrentPage];
		uint64 AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);

		if (Page->AllocatedBytes + AlignmentOffset + Size > Page->SizeBytes)
		{
			for (uint64 Index = m_CurrentPage + 1; Index < m_Pages.Size(); Index++)
			{
				Page = m_Pages[Index];
				AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
				if (Page->AllocatedBytes + AlignmentOffset + Size <= Page->SizeBytes)
				{
					m_CurrentPage = Index;
					Page->AllocatedBytes += (AlignmentOffset + Size);
					*OutPointer = (uint8*)Page->MemoryBlock + AlignmentOffset;
					return (int16)EMemoryError::Success;
				}
			}

			if (!m_Specification.bShouldGrow)
			{
				*OutPointer = nullptr;
				return (int16)EMemoryError::OutOfMemoryUnableToGrow;
			}

			Page = AllocateNewPage(GetOptimalPageSize(Size));
			m_CurrentPage = m_Pages.Size() - 1;

			AlignmentOffset = GetAlignmentOffset((uint8*)Page->MemoryBlock + Page->AllocatedBytes, Alignment);
		}

		Page->AllocatedBytes += (AlignmentOffset + Size);
		*OutPointer = (uint8*)Page->MemoryBlock + AlignmentOffset;
		return (int16)EMemoryError::Success;
	}

	NODISCARD void* ALinearArena::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		return nullptr;
	}

	void ALinearArena::Free(void* Allocation, uint64 Size)
	{
		switch (m_FailureMode)
		{
			case AMemoryArena::EFailureMode::Assert:
				AE_CHECK_NO_ENTRY();
				break;
			case AMemoryArena::EFailureMode::Error:
				AE_CORE_WARN(TEXT("A Linear Arena can't free specific allocations!"));
				break;
		}
	}

	int32 ALinearArena::TryFree(void* Allocation, uint64 Size)
	{
		return (int16)EMemoryError::InvalidCall;
	}

	void ALinearArena::FreeUnsafe(void* Allocation, uint64 Size)
	{
	}

	void ALinearArena::FreeAll()
	{
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
		m_CurrentPage = 0;
	}

	int32 ALinearArena::TryFreeAll()
	{
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
		m_CurrentPage = 0;
		return (int16)EMemoryError::Success;
	}

	void ALinearArena::FreeAllUnsafe()
	{
		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			m_Pages[Index]->AllocatedBytes = 0;
		}
		m_CurrentPage = 0;
	}

	void ALinearArena::GarbageCollect()
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

	uint64 ALinearArena::GetOptimalPageSize(uint64 RequestedAllocationSize) const
	{
		return m_Pages.Back()->SizeBytes;
	}

	ALinearArena::APage* ALinearArena::AllocateNewPage(uint64 PageSize)
	{
		if (m_Specification.bUseArenaMemoryAlways)
		{
			APage* NewPage = Utils::ConstructNewPage((uint8*)m_Specification.ArenaMemory, m_Specification.ArenaMemoryOffset, PageSize);
			m_Pages.PushBack(NewPage);
			return NewPage;
		}
		else
		{
			uint64 Offset = 0;
			APage* NewPage = Utils::ConstructNewPage((uint8*)GMalloc->Alloc(GetPageMemoryRequirement(PageSize)), Offset, PageSize);
			m_Pages.PushBack(NewPage);
			return NewPage;
		}
	}

}