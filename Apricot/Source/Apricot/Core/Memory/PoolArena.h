// Part of Apricot Engine. 2022-2022.

#pragma once

#include "ApricotMemory.h"

namespace Apricot {

	/**
	* C++ Core Engine Architecture
	* 
	* Memory Arena based on pool memory allocation strategy.
	* Very efficient if the requested allocations are nearly the same size.
	* 
	* It is recommended, for peak efficiency, that chunk size is bigger than 2 * sizeof(void*).
	* Currently, it doesn't have the ability to resize.
	*/
	class APRICOT_API APoolArena : public AMemoryArena
	{
	/* Constructors & Deconstructor */
	private:
		APoolArena();
		virtual ~APoolArena() override = default;

		APoolArena(const APoolArena&) = delete;
		APoolArena(APoolArena&&) = delete;
		APoolArena& operator=(const APoolArena&) = delete;
		APoolArena& operator=(APoolArena&&) = delete;
	
	/* API interface */
	public:
		/**
		* Returns the total size of the used chunks.
		* Might not be accurate with the requested allocations' sizes.
		*/
		virtual uint64 GetAllocatedSize() const override;

		/**
		* Returns the debug tag of the arena.
		*/
		virtual const TChar* GetDebugName() const override;

		/**
		* Returns the total size of the free chunks.
		*/
		virtual uint64 GetFreeSize() const override;

		/**
		* Returns the total size of all chunks (free or not).
		*/
		virtual uint64 GetTotalSize() const override;

		/**
		* Allocates from a single chunk of memory. Because of this, the maximum size is the chunk size (but this ignores alignment).
		* Generates errors based on the EFailureMode enum value. Always return nullptr on error.
		* 
		* @param Size The size of the allocation. Less or equal to the size of a chunk, when alignment of 1 is specified. Otherwise,
		*			it is recommended to request maximum of ChunkSize - Alignment.
		* 
		* @param Alignment The value that the returned pointer is aligned to. This is made possible by padding from the beginning of the chunk.
		*			Default value is sizeof(void*), which is fine on most of the platforms.
		* 
		* @returns A pointer that is guaranteed to have the specified Size and aligned to the specified Alignment.
		*/
		NODISCARD virtual void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*)) override;

		/**
		* Allocates from a single chunk of memory. Because of this, the maximum size is the chunk size (but this ignores alignment).
		* Main difference from 'Alloc' is that it returns a flag specifying the encountered error. The outer pointer is set to nullptr on error.
		* 
		* @param Size The size of the allocation. Less or equal to the size of a chunk, when alignment of 1 is specified. Otherwise,
		*			it is recommended to request maximum of ChunkSize - Alignment.
		* 
		* @param OutPointer Pointer to the pointer that points to the requested memory. Any previous value will be overwritten. MUST not be nullptr.
		*
		* @param Alignment The value that the returned pointer is aligned to. This is made possible by padding from the beginning of the chunk.
		*			Default value is sizeof(void*), which is fine on most of the platforms.
		* 
		* @returns A flag specifying if any errors were encountered. A simple 'if' statement will check for any error flags.
		*				All possible error return flags: AE_ALLOC_BAD_ARENA, AE_ALLOC_INVALID_SIZE, AE_ALLOC_OUT_OF_MEMORY.
		*/
		NODISCARD virtual int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*)) override;

		/**
		* Allocates from a single chunk of memory. Because of this, the maximum size is the chunk size (but this ignores alignment).
		* It doesn't perform ANY error checking and will most likely unreportedly crash the engine if ANY errors occur.
		* 
		* @param Size The size of the allocation. Less or equal to the size of a chunk, when alignment of 1 is specified. Otherwise,
		*			it is recommended to request maximum of ChunkSize - Alignment.
		*
		* @param Alignment The value that the returned pointer is aligned to. This is made possible by padding from the beginning of the chunk.
		*			Default value is sizeof(void*), which is fine on most of the platforms.
		*
		* @returns A pointer that is guaranteed to have the specified Size and aligned to the specified Alignment. If the function doesn't fail, of course.
		*/
		NODISCARD virtual void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*)) override;

		/**
		* Frees the chunk where Allocation is placed.
		* Generates errors based on EFailureMode enum value.
		* 
		* @param Allocation Pointer to the memory to be freed.
		* 
		* @param Size Size of the allocation. Currently, used only for debugging.
		*/
		virtual void Free(void* Allocation, uint64 Size) override;

		/**
		* Frees the chunk where Allocation is placed.
		* Same behavior as 'Free', but it returns a flag specifying if any errors were encountered.
		*
		* @param Allocation Pointer to the memory to be freed.
		*
		* @param Size Size of the allocation. Currently, used only for debugging.
		* 
		* @returns A flag specifying if any errors were encountered. A simple 'if' statement will check for any error flags.
		*				All possible error return flags: AE_FREE_BAD_ARENA, AE_FREE_POINTER_OUT_OF_RANGE, AE_FREE_ALREADY_FREED.
		*/
		virtual int32 TryFree(void* Allocation, uint64 Size) override;

		/**
		* Frees the chunk where Allocation is placed.
		* It doesn't perform ANY error checking and will most likely unreportedly crash the engine if ANY errors occur.
		*
		* @param Allocation Pointer to the memory to be freed.
		*
		* @param Size Size of the allocation. Currently, used only for debugging.
		*/
		virtual void FreeUnsafe(void* Allocation, uint64 Size) override;

	private:
		/**
		* Pointer to the memory that holds the chunks.
		*/
		void* m_MemoryBlock;

		/**
		* Total size of the chunks.
		*/
		uint64 m_TotalSizeBytes;

		/**
		* Size of a single chunk.
		*/
		uint64 m_ChunkSizeBytes;

		/**
		* The memory used might be allocated from somewhere else, so we have to keep track if the arena allocated its memory on creation or not.
		*/
		bool8 m_bOwnsMemory;

		/**
		* Array of pointers to each free chunk. Each chunk should have one freelist spot.
		*/
		void** m_FreeChunks;

		/**
		* Count of free chunks.
		*/
		uint64 m_FreeChunksCount;

	private:
		friend APRICOT_API APoolArena* CreatePoolArena(uint64, uint64, void*, uint64*);
		friend APRICOT_API void DestroyPoolArena(APoolArena*);

		template<typename T, typename... Args>
		friend T* MemConstruct(void*, Args&&...);
	};

	/**
	* Allocate & Create a Pool Arena.
	* 
	* @param ChunksCount The count of the pool's total chunks.
	* 
	* @param ChunkSizeBytes The size of a chunks, in bytes.
	* 
	* @param Memory Optional parameter that provides the pool with memory. If this is nullptr, the pool with allocate on the global heap, via GMalloc, the required memory.
	*			Default value is nullptr.
	* 
	* @param OutMemoryRequirement Optional outer parameter that indicates how much memory was actually allocated/used. Default value is nullptr.
	* 
	* @returns Pointer to the freshly created pool.
	*/
	NODISCARD APRICOT_API APoolArena* CreatePoolArena(uint64 ChunksCount, uint64 ChunkSizeBytes, void* Memory = nullptr, uint64* OutMemoryRequirement = nullptr);

	/**
	* Destroys a Pool Arena.
	*/
	APRICOT_API void DestroyPoolArena(APoolArena* Pool);

	/**
	* Calculates how much memory is actually required for creating a pool arena (Chunks, Freelist alignment, Freelist).
	* This function is also called internally when 'CreatePoolArena', the return value being placed in the OutMemoryRequirement outer parameter.
	*/
	APRICOT_API uint64 GetPoolArenaMemoryRequirement(uint64 ChunksCount, uint64 ChunkSizeBytes);

}