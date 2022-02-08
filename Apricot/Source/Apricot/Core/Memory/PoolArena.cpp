#include "aepch.h"
#include "PoolArena.h"

namespace Apricot {

	APoolArena::APoolArena()
	{
		
	}

	uint64 APoolArena::GetAllocatedSize() const
	{
		return m_TotalSizeBytes - m_FreeChunksCount * m_ChunkSizeBytes;
	}

	const TChar* APoolArena::GetDebugName() const
	{
		return TEXT("Base_PoolArena");
	}

	uint64 APoolArena::GetFreeSize() const
	{
		return m_FreeChunksCount * m_ChunkSizeBytes;
	}

	uint64 APoolArena::GetTotalSize() const
	{
		m_TotalSizeBytes;
	}

	void* APoolArena::Alloc(uint64 Size)
	{
		if (Size > m_ChunkSizeBytes)
		{
			/* TODO: Handle error */
			return nullptr;
		}
		if (m_FreeChunksCount == 0)
		{
			/* TODO: Handle error */
			return nullptr;
		}

		return m_FreeChunks[m_FreeChunksCount--];
	}

	int32 APoolArena::TryAlloc(uint64 Size, void** OutPointer)
	{
		if (Size > m_ChunkSizeBytes || Size == 0)
		{
			return AE_ALLOC_INVALID_SIZE;
		}
		if (m_FreeChunksCount == 0)
		{
			return AE_ALLOC_OUT_OF_MEMORY;
		}

		*OutPointer = m_FreeChunks[m_FreeChunksCount--];
		return AE_ALLOC_SUCCESSFULLY;
	}

	void APoolArena::Free(void* Allocation)
	{
		if (m_MemoryBlock > Allocation || Allocation >= (uint8_t*)m_MemoryBlock + m_TotalSizeBytes)
		{
			/* Pointer out of range! */
			return;
		}
		if (((uint8_t*)Allocation - (uint8_t*)m_MemoryBlock) % m_ChunkSizeBytes != 0)
		{
			/* Pointer doesn't match any pool's memory locations */
			return;
		}
		if (m_FreeChunksCount == m_TotalSizeBytes / m_ChunkSizeBytes)
		{
			/* Pool chunk was already freed */
			return;
		}

		m_FreeChunks[m_FreeChunksCount++] = Allocation;
	}

	int32 APoolArena::TryFree(void* Allocation)
	{
		if (m_MemoryBlock > Allocation || Allocation >= (uint8_t*)m_MemoryBlock + m_TotalSizeBytes)
		{
			return AE_FREE_POINTER_OUT_OF_RANGE;
		}
		if (((uint8_t*)Allocation - (uint8_t*)m_MemoryBlock) % m_ChunkSizeBytes != 0)
		{
			return AE_FREE_BAD_POINTER;
		}
		if (m_FreeChunksCount == m_TotalSizeBytes / m_ChunkSizeBytes)
		{
			return AE_FREE_ALREADY_FREED;
		}

		m_FreeChunks[m_FreeChunksCount++] = Allocation;
		return AE_FREE_SUCCESSFULLY;
	}

}