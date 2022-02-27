// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "PoolArena.h"

namespace Apricot {

	namespace Utils {
		
		static APoolArena::APage* ConstructNewPage(uint8* ArenaMemory, uint64& Offset, uint64 ChunksCount, uint64 ChunkSize)
		{
			Offset += GetAlignmentOffset(Offset, sizeof(void*));

			APoolArena::APage* NewPage = (APoolArena::APage*)(ArenaMemory + Offset);
			MemConstruct<APoolArena::APage>(NewPage);
			Offset += sizeof(APoolArena::APage);

			NewPage->FreeChunksCount = ChunksCount;
			NewPage->FreeChunks = (void**)(ArenaMemory + Offset);
			Offset += NewPage->FreeChunksCount * sizeof(void*);

			NewPage->MemoryBlock = ArenaMemory + Offset;
			NewPage->ChunksCount = ChunksCount;
			NewPage->ChunkSize = ChunkSize;

			for (uint64 Index = 0; Index < NewPage->FreeChunksCount; Index++)
			{
				NewPage->FreeChunks[Index] = ArenaMemory + Offset;
				Offset += NewPage->ChunkSize;
			}

			return NewPage;
		}

	}

	TSharedPtr<APoolArena> APoolArena::Create(const APoolArenaSpecification& Specification)
	{
		return MakeShared<APoolArena>(Specification);
	}

	NODISCARD uint64 APoolArena::GetMemoryRequirement(const APoolArenaSpecification& Specification)
	{
		uint64 MemoryRequirement = 0;

		for (uint64 Index = 0; Index < Specification.PagesCount; Index++)
		{
			MemoryRequirement += GetAlignmentOffset(MemoryRequirement, sizeof(void*));
			MemoryRequirement += GetPageMemoryRequirement(Specification.PageChunkCounts[Index], Specification.PageChunkSizes[Index]);
		}

		return MemoryRequirement;
	}

	NODISCARD uint64 APoolArena::GetPageMemoryRequirement(uint64 ChunksCount, uint64 ChunkSize)
	{
		uint64 MemoryRequirement = 0;

		MemoryRequirement += sizeof(APage);
		MemoryRequirement += ChunksCount * sizeof(void*);
		MemoryRequirement += ChunksCount * ChunkSize;

		return MemoryRequirement;
	}

	APoolArena::APoolArena(const APoolArenaSpecification& Specification)
		: m_Specification(Specification)
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
				m_Pages.PushBack(Utils::ConstructNewPage(ArenaMemory, MemoryOffset, m_Specification.PageChunkCounts[Index], m_Specification.PageChunkSizes[Index]));
			}
		}
		else
		{
			for (uint64 Index = 0; Index < m_Specification.PagesCount; Index++)
			{
				uint64 MemoryOffset = 0;
				uint64 PageChunksCount = m_Specification.PageChunkCounts[Index];
				uint64 PageChunkSize = m_Specification.PageChunkSizes[Index];

				ArenaMemory = (uint8*)GMalloc->Alloc(GetPageMemoryRequirement(PageChunksCount, PageChunkSize));
				m_Pages.PushBack(Utils::ConstructNewPage(ArenaMemory, MemoryOffset, PageChunksCount, PageChunkSize));
			}
		}
	}

	APoolArena::~APoolArena()
	{
		if (!m_Specification.bUseArenaMemoryAlways)
		{
			for (uint64 Index = m_Specification.PagesCount; Index < m_Pages.Size(); Index++)
			{
				GMalloc->Free(m_Pages[Index], GetPageMemoryRequirement(m_Pages[Index]->ChunksCount, m_Pages[Index]->ChunkSize));
			}
		}
		if (!m_Specification.ArenaMemory && !m_Pages.IsEmpty())
		{
			GMalloc->Free(m_Pages[0], GetMemoryRequirement(m_Specification));
		}
	}

	uint64 APoolArena::GetAllocatedSize() const
	{
		uint64 AllocatedSize = 0;

		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			AllocatedSize += (m_Pages[Index]->ChunksCount - m_Pages[Index]->FreeChunksCount) * m_Pages[Index]->ChunkSize;
		}

		return AllocatedSize;
	}

	const TChar* APoolArena::GetDebugName() const
	{
		return TEXT("POOL_ARENA");
	}

	uint64 APoolArena::GetFreeSize() const
	{
		uint64 FreeSize = 0;

		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			FreeSize += m_Pages[Index]->FreeChunksCount * m_Pages[Index]->ChunkSize;
		}

		return FreeSize;
	}

	uint64 APoolArena::GetTotalSize() const
	{
		uint64 TotalSize = 0;

		for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
		{
			TotalSize += m_Pages[Index]->ChunksCount * m_Pages[Index]->ChunkSize;
		}

		return TotalSize;
	}

	NODISCARD void* APoolArena::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*)*/, EAllocStrategy Mode /*= EFindMode::BestFit*/)
	{
		if (Mode == EAllocStrategy::BestFit)
		{
			uint64 MinimumSize = AE_UINT64_MAX;
			APage* BestPage = nullptr;

			for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
			{
				if (m_Pages[Index]->FreeChunksCount > 0 && Size + Alignment - 1 <= m_Pages[Index]->ChunkSize)
				{
					if (m_Pages[Index]->ChunkSize < MinimumSize)
					{
						MinimumSize = m_Pages[Index]->ChunkSize;
						BestPage = m_Pages[Index];
						if (MinimumSize == Size + Alignment - 1)
						{
							break;
						}
					}
				}
			}

			if (BestPage != nullptr)
			{
				uint8* Memory = (uint8*)BestPage->FreeChunks[--BestPage->FreeChunksCount];
				uint64 AlignmentOffset = GetAlignmentOffset(Memory, Alignment);
				return Memory + AlignmentOffset;
			}
		}
		else if (Mode == EAllocStrategy::FirstFit)
		{
			for (uint64 Index = 0; Index < m_Pages.Size(); Index++)
			{
				if (m_Pages[Index]->FreeChunksCount > 0 && Size + Alignment - 1 <= m_Pages[Index]->ChunkSize)
				{
					uint8* Memory = (uint8*)m_Pages[Index]->FreeChunks[--m_Pages[Index]->FreeChunksCount];
					uint64 AlignmentOffset = GetAlignmentOffset(Memory, Alignment);
					return Memory + AlignmentOffset;
				}
			}
		}

		if (!m_Specification.bShouldGrow)
		{
			// TODO: Report error
			return nullptr;
		}

		// Arbitrary number
		// TODO (Avr): Think about a better way of calculating these values 
		uint64 NewPageChunksCount = 32;
		uint64 NewPageChunkSize = Size + Alignment - 1;
		if (!m_Pages.IsEmpty())
		{
			uint64 MedianChunksCount = 0;
			uint64 MedianChunkSize = 0;
			for (const APage* Page : m_Pages)
			{
				MedianChunksCount += Page->ChunksCount;
				MedianChunkSize += Page->ChunkSize;
			}
			MedianChunksCount /= m_Pages.Size();
			MedianChunkSize /= m_Pages.Size();

			NewPageChunksCount = MedianChunksCount;
			if (NewPageChunkSize < MedianChunkSize)
			{
				NewPageChunkSize = MedianChunkSize;
			}
		}

		AllocateNewPage(NewPageChunksCount, NewPageChunkSize);
		APage* NewPage = m_Pages.Back();

		uint8* Memory = (uint8*)NewPage->FreeChunks[--NewPage->FreeChunksCount];
		uint64 AlignmentOffset = GetAlignmentOffset(Memory, Alignment);
		return Memory + AlignmentOffset;
	}

	NODISCARD int32 APoolArena::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/, EAllocStrategy Mode /*= EFindMode::BestFit*/)
	{
		return (int32)EMemoryError::Success;
	}

	NODISCARD void* APoolArena::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/, EAllocStrategy Mode /*= EFindMode::BestFit*/)
	{
		return nullptr;
	}

	void APoolArena::Free(void* Allocation, uint64 Size)
	{
		
	}

	int32 APoolArena::TryFree(void* Allocation, uint64 Size)
	{
		return (int32)EMemoryError::Success;
	}

	void APoolArena::FreeUnsafe(void* Allocation, uint64 Size)
	{
		
	}

	void APoolArena::FreeAll()
	{
		
	}

	int32 APoolArena::TryFreeAll()
	{
		return (int32)EMemoryError::Success;
	}

	void APoolArena::FreeAllUnsafe()
	{
		
	}

	void APoolArena::GarbageCollect()
	{
		for (int64 Index = m_Pages.Size() - 1; Index >= (int64)m_Specification.PagesCount; Index--)
		{
			APage* Page = m_Pages[Index];
			if (Page->FreeChunksCount == Page->ChunksCount)
			{
				if (!m_Specification.bUseArenaMemoryAlways)
				{
					GMalloc->Free(Page, GetPageMemoryRequirement(Page->ChunksCount, Page->ChunkSize));
				}

				m_Pages.Erase(Index);
			}
		}

		// TODO (Avr): Garbage collect the specification pages as well
	}

	void APoolArena::AllocateNewPage(uint64 ChunksCount, uint64 ChunkSize)
	{
		if (m_Specification.bUseArenaMemoryAlways)
		{
			m_Pages.PushBack(Utils::ConstructNewPage((uint8*)m_Specification.ArenaMemory, m_Specification.ArenaMemoryOffset, ChunksCount, ChunkSize));
		}
		else
		{
			uint8* Memory = (uint8*)GMalloc->Alloc(GetPageMemoryRequirement(ChunksCount, ChunkSize));
			uint64 Offset = 0;
			m_Pages.PushBack(Utils::ConstructNewPage(Memory, Offset, ChunksCount, ChunkSize));
		}
	}

}