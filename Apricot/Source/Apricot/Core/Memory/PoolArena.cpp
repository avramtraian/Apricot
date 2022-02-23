// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "PoolArena.h"

namespace Apricot {

	APoolArena::APoolArena()
		: m_MemoryBlock(nullptr), m_TotalSizeBytes(AE_INVALID_MEMSIZE), m_ChunkSizeBytes(AE_INVALID_MEMSIZE), m_bOwnsMemory(false), m_FreeChunks(nullptr), m_FreeChunksCount(0)
	{
	}

	uint64 APoolArena::GetAllocatedSize() const
	{
		return m_TotalSizeBytes - m_FreeChunksCount * m_ChunkSizeBytes;
	}

	const TChar* APoolArena::GetDebugName() const
	{
		return TEXT("Base_POOLARENA");
	}

	uint64 APoolArena::GetFreeSize() const
	{
		return m_FreeChunksCount * m_ChunkSizeBytes;
	}

	uint64 APoolArena::GetTotalSize() const
	{
		return m_TotalSizeBytes;
	}

	void* APoolArena::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (!m_MemoryBlock)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Invalid PoolArena */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Invalid PoolArena. Returning nullptr."));
					break;
			}
			return nullptr;
		}
		if (Size > m_ChunkSizeBytes)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Pool chunk too small for this allocation */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Pool chunk too small for the requested allocation. Returning nullptr."));
					break;
			}
			return nullptr;
		}
		if (m_FreeChunksCount == 0)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Pool arena out of memory */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Pool arena out of memory. Returning nullptr."));
					break;
			}
			return nullptr;
		}

		void* Chunk = m_FreeChunks[--m_FreeChunksCount];
		uint64 AlignmentOffset = (Alignment - (uintptr)Chunk % Alignment) % Alignment;
		if (Size + AlignmentOffset > m_ChunkSizeBytes)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Pool chunk too small for the requested allocation, taking into account the alignment */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Pool chunk too small for the requested allocation, taking into account the alignment. Returning nullptr."));
					break;
			}
			return nullptr;
		}
		return (uint8*)Chunk + AlignmentOffset;
	}

	int32 APoolArena::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (!m_MemoryBlock)
		{
			return (int16)EMemoryError::InvalidArena;
		}
		if (Size > m_ChunkSizeBytes || Size == 0)
		{
			return (int16)EMemoryError::InvalidSize;
		}
		if (m_FreeChunksCount == 0)
		{
			return (int16)EMemoryError::OutOfMemory;
		}

		void* Chunk = m_FreeChunks[--m_FreeChunksCount];
		uint64 AlignmentOffset = (Alignment - (uintptr)Chunk % Alignment) % Alignment;
		if (Size + AlignmentOffset > m_ChunkSizeBytes)
		{
			m_FreeChunksCount++;
			/* TODO: Information about the fact that this error is triggered due to alignment */
			return (int16)EMemoryError::InvalidSize;
		}
		*OutPointer = (uint8*)Chunk + AlignmentOffset;
		return (int16)EMemoryError::Success;
	}

	void* APoolArena::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		void* Chunk = m_FreeChunks[--m_FreeChunksCount];
		uint64 AlignmentOffset = (Alignment - (uintptr)Chunk % Alignment) % Alignment;
		return (uint8*)Chunk + AlignmentOffset;
	}

	void APoolArena::Free(void* Allocation, uint64 Size)
	{
		if (!m_MemoryBlock)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Invalid PoolArena */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Invalid PoolArena. Returning."));
					break;
			}
			return;
		}
		if (m_MemoryBlock > Allocation || Allocation >= (uint8_t*)m_MemoryBlock + m_TotalSizeBytes)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* Pointer out of range */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - Pointer out of range. Returning."));
					break;
			}
			return;
		}
		if (m_FreeChunksCount == m_TotalSizeBytes / m_ChunkSizeBytes)
		{
			switch (m_FailureMode)
			{
				case AMemoryArena::EFailureMode::Assert:
					AE_CHECK(false); /* There are no allocated chunks (to free) */
					break;
				case AMemoryArena::EFailureMode::Error:
					AE_CORE_ERROR(TEXT("PoolArena failure - There are no allocated chunks (to free). Returning."));
					break;
			}
			return;
		}

		void* AllocationChunk = (uint8*)Allocation - ((uintptr)Allocation - (uintptr)m_MemoryBlock) % m_ChunkSizeBytes;
		m_FreeChunks[m_FreeChunksCount++] = AllocationChunk;
	}

	int32 APoolArena::TryFree(void* Allocation, uint64 Size)
	{
		if (!m_MemoryBlock)
		{
			return (int16)EMemoryError::InvalidArena;
		}
		if (m_MemoryBlock > Allocation || Allocation >= (uint8_t*)m_MemoryBlock + m_TotalSizeBytes)
		{
			return (int16)EMemoryError::PointerOutOfRange;
		}
		if (m_FreeChunksCount == (m_TotalSizeBytes / m_ChunkSizeBytes))
		{
			return (int16)EMemoryError::AlreadyFreed;
		}

		void* AllocationChunk = (uint8*)Allocation - ((uintptr)Allocation - (uintptr)m_MemoryBlock) % m_ChunkSizeBytes;
		m_FreeChunks[m_FreeChunksCount++] = AllocationChunk;
		return (int16)EMemoryError::Success;
	}

	void APoolArena::FreeUnsafe(void* Allocation, uint64 Size)
	{
		void* AllocationChunk = (uint8*)Allocation - ((uintptr)Allocation - (uintptr)m_MemoryBlock) % m_ChunkSizeBytes;
		m_FreeChunks[m_FreeChunksCount++] = AllocationChunk;
	}

	void APoolArena::FreeAll()
	{
		
	}

	int32 APoolArena::TryFreeAll()
	{
		return (int16)EMemoryError::InvalidCall;
	}

	void APoolArena::FreeAllUnsafe()
	{
		
	}

	void APoolArena::GarbageCollect()
	{
		
	}

	APRICOT_API APoolArena* CreatePoolArena(uint64 ChunksCount, uint64 ChunkSizeBytes, void* Memory /*= nullptr*/, uint64* OutMemoryRequirement /*= nullptr*/)
	{
		/* TODO: Allocate the actual pool arena */
		APoolArena* Pool = (APoolArena*)GMalloc->Alloc(sizeof(APoolArena));
		MemConstruct<APoolArena>(Pool);

		Pool->m_TotalSizeBytes = ChunksCount * ChunkSizeBytes;
		Pool->m_ChunkSizeBytes = ChunkSizeBytes;
		Pool->m_FreeChunksCount = ChunksCount;

		uint64 PoolMemorySize = GetPoolArenaMemoryRequirement(ChunksCount, ChunkSizeBytes);
		if (OutMemoryRequirement)
		{
			*OutMemoryRequirement = PoolMemorySize;
		}

		if (Memory)
		{
			Pool->m_MemoryBlock = Memory;
			Pool->m_bOwnsMemory = false;
		}
		else
		{
			Pool->m_MemoryBlock = GMalloc->Alloc(PoolMemorySize, sizeof(void*));
			Pool->m_bOwnsMemory = true;
		}
		Pool->m_FreeChunks = (void**)((uint8*)Pool->m_MemoryBlock + PoolMemorySize - (ChunksCount * sizeof(void*)));

		for (uint64 Index = 0; Index < Pool->m_FreeChunksCount; Index++)
		{
			Pool->m_FreeChunks[Index] = (uint8*)Pool->m_MemoryBlock + Pool->m_ChunkSizeBytes * Index;
		}

		return Pool;
	}

	APRICOT_API void DestroyPoolArena(APoolArena* Pool)
	{
		if (Pool)
		{
			if (Pool->m_bOwnsMemory)
			{
				void* PoolMemory = Pool->m_MemoryBlock;
				uint64 PoolMemorySize = ((uintptr)Pool->m_FreeChunks + Pool->m_FreeChunksCount * sizeof(void*)) - (uintptr)Pool->m_MemoryBlock;
				if (PoolMemory && PoolMemorySize > 0)
				{
					GMalloc->Free(PoolMemory, PoolMemorySize);
				}
			}

			Pool->~APoolArena();
			GMalloc->Free(Pool, sizeof(APoolArena));
		}
	}

	APRICOT_API uint64 GetPoolArenaMemoryRequirement(uint64 ChunksCount, uint64 ChunkSizeBytes)
	{
		uint64 TotalSize = ChunksCount * ChunkSizeBytes;
		uint64 FreelistAlignment = (sizeof(void*) - TotalSize % sizeof(void*)) % sizeof(void*);
		return
			TotalSize +                  /* Memory storage */
			FreelistAlignment +          /* Alignment for the freelist */
			ChunksCount * sizeof(void*); /* Freelist */
	}

}