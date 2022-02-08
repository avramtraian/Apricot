#pragma once

#include "ApricotMemory.h"

namespace Apricot {

	class APRICOT_API APoolArena : public AMemoryArena
	{
	private:
		APoolArena();
		APoolArena(const APoolArena&) = delete;
		APoolArena& operator=(const APoolArena&) = delete;
		virtual ~APoolArena() override = default;

	public:
		virtual uint64 GetAllocatedSize() const override;
		virtual const TChar* GetDebugName() const override;
		virtual uint64 GetFreeSize() const override;
		virtual uint64 GetTotalSize() const override;

		virtual void* Alloc(uint64 Size) override;
		virtual int32 TryAlloc(uint64 Size, void** OutPointer) override;

		virtual void Free(void* Allocation) override;
		virtual int32 TryFree(void* Allocation) override;

	private:
		void* m_MemoryBlock;
		uint64 m_TotalSizeBytes;
		uint64 m_ChunkSizeBytes;
		bool8 m_bOwnsMemory;

		void** m_FreeChunks;
		uint64 m_FreeChunksCount;
	};

	APRICOT_API APoolArena* CreatePoolArena(uint64 ChunksCount, uint64 ChunkSizeBytes, void* Memory = nullptr, bool8 bMemoryIncludesFreelist = true);
	APRICOT_API void DestroyPoolArena(APoolArena* Pool);

}