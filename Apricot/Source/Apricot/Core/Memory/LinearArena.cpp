// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "LinearArena.h"

namespace Apricot {

	ALinearArena::ALinearArena()
		: m_CurrentPage(0)
	{
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
		uint8* Memory = (uint8*)GMalloc->Alloc(sizeof(APage) + PageSize);

		APage* NewPage = (APage*)(Memory);
		MemConstruct<APage>(NewPage);

		NewPage->MemoryBlock = Memory + sizeof(APage);
		NewPage->SizeBytes = PageSize;
		NewPage->AllocatedBytes = 0;

		m_Pages.PushBack(NewPage);
		return NewPage;
	}

	APRICOT_API uint64 GetLinearArenaMemoryRequirementEx(const ALinearArenaSpecification& Specification)
	{
		uint64 MemoryRequirement = 0;

		MemoryRequirement += sizeof(ALinearArena);

		for (uint64 Index = 0; Index < Specification.PagesCount; Index++)
		{
			MemoryRequirement += GetAlignmentOffset(MemoryRequirement, sizeof(void*));;
			MemoryRequirement += sizeof(ALinearArena::APage);
			MemoryRequirement += Specification.PageSizes[Index];
		}

		return MemoryRequirement;
	}

	APRICOT_API uint64 GetLinearArenaMemoryRequirement(uint64 SizeBytes)
	{
		ALinearArenaSpecification Specification;
		Specification.PagesCount = 1;
		Specification.PageSizes = &SizeBytes;

		return GetLinearArenaMemoryRequirementEx(Specification);
	}

	APRICOT_API uint64 GetLinearArenaMemoryRequirement(uint64 PagesCount, uint64* PageSizes)
	{
		ALinearArenaSpecification Specification;
		Specification.PagesCount = PagesCount;
		Specification.PageSizes = PageSizes;

		return GetLinearArenaMemoryRequirementEx(Specification);
	}

	APRICOT_API ALinearArena* CreateLinearArenaEx(const ALinearArenaSpecification& Specification)
	{
		uint8* ArenaMemory = (uint8*)Specification.ArenaMemory;
		if (!ArenaMemory)
		{
			ArenaMemory = (uint8*)GMalloc->Alloc(GetLinearArenaMemoryRequirementEx(Specification));
		}
		uint64 MemoryOffset = 0;

		ALinearArena* Arena = (ALinearArena*)(ArenaMemory + MemoryOffset);
		MemConstruct<ALinearArena>(Arena);

		Arena->m_Specification = Specification;
		Arena->m_Pages.Reserve(Specification.PagesCount);
		Arena->m_CurrentPage = 0;

		MemoryOffset += sizeof(ALinearArena);
		MemoryOffset += GetAlignmentOffset(MemoryOffset, sizeof(void*));

		for (uint64 Index = 0; Index < Specification.PagesCount; Index++)
		{
			uint64 PageAlignment = GetAlignmentOffset(MemoryOffset, sizeof(void*));
			MemoryOffset += PageAlignment;

			ALinearArena::APage* Page = (ALinearArena::APage*)(ArenaMemory + MemoryOffset);
			Arena->m_Pages.PushBack(Page);

			MemoryOffset += sizeof(ALinearArena::APage);

			Page->MemoryBlock = ArenaMemory + MemoryOffset;
			Page->SizeBytes = Specification.PageSizes[Index];
			Page->AllocatedBytes = 0;

			MemoryOffset += Specification.PageSizes[Index];
		}

		return Arena;
	}

	APRICOT_API ALinearArena* CreateLinearArena(uint64 SizeBytes, bool8 bShouldGrow, void* Memory /*= nullptr*/)
	{
		ALinearArenaSpecification Specification;
		Specification.PagesCount = 1;
		Specification.PageSizes = &SizeBytes;
		Specification.bShouldGrow = bShouldGrow;
		Specification.ArenaMemory = Memory;

		return CreateLinearArenaEx(Specification);
	}

	APRICOT_API ALinearArena* CreateLinearArena(uint64 PagesCount, uint64* PageSizes, bool8 bShouldGrow, void* Memory /*= nullptr*/)
	{
		ALinearArenaSpecification Specification;
		Specification.PagesCount = PagesCount;
		Specification.PageSizes = PageSizes;
		Specification.bShouldGrow = bShouldGrow;
		Specification.ArenaMemory = Memory;

		return CreateLinearArenaEx(Specification);
	}

	APRICOT_API void DestroyLinearArena(ALinearArena* Arena)
	{
		if (Arena)
		{
			for (uint64 Index = Arena->m_Specification.PagesCount; Index < Arena->m_Pages.Size(); Index++)
			{
				uint64 PageSize = (uint64)((uintptr)Arena->m_Pages[Index]->MemoryBlock - (uintptr)Arena->m_Pages[Index]) + Arena->m_Pages[Index]->SizeBytes;
				GMalloc->Free(Arena->m_Pages[Index], PageSize);
			}

			uint64 ArenaSize = GetLinearArenaMemoryRequirementEx(Arena->m_Specification);
			bool8 bOwnsMemory = (Arena->m_Specification.ArenaMemory == nullptr);
			Arena->~ALinearArena();
			if (bOwnsMemory)
			{
				GMalloc->Free(Arena, ArenaSize);
			}
		}
	}

}