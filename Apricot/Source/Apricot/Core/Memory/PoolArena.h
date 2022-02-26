// Part of Apricot Engine. 2022-2022.

#pragma once

#include "ApricotMemory.h"

#include "Apricot/Containers/SharedPtr.h"

namespace Apricot {

	/**
	* 
	*/
	struct APoolArenaSpecification
	{
		uint64 PagesCount = 0;

		uint64* PageChunkCounts = nullptr;

		uint64* PageChunkSizes = nullptr;

		void* ArenaMemory = nullptr;

		uint64 ArenaMemoryOffset = 0;

		bool bShouldGrow = true;

		bool bBulkAllocateSpecPages = true;

		bool bUseArenaMemoryAlways = false;
	};

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
	public:
		NODISCARD static TSharedPtr<APoolArena> Create(const APoolArenaSpecification& Specification);

		NODISCARD static uint64 GetMemoryRequirementEx(const APoolArenaSpecification& Specification);

		NODISCARD static uint64 GetPageMemoryRequirement(uint64 ChunksCount, uint64 ChunkSize);

	/* Constructors & Deconstructor */
	private:
		APoolArena(const APoolArenaSpecification& Specification);
		virtual ~APoolArena() override;

		APoolArena(const APoolArena&) = delete;
		APoolArena(APoolArena&&) = delete;
		APoolArena& operator=(const APoolArena&) = delete;
		APoolArena& operator=(APoolArena&&) = delete;

	/* Typedefs */
	public:
		struct APage
		{
			void* MemoryBlock = nullptr;

			uint64 ChunksCount = 0;

			uint64 ChunkSize = 0;

			void** FreeChunks = nullptr;

			uint64 FreeChunksCount = 0;
		};
	
	/* API interface */
	public:
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
		NODISCARD void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*), EAllocStrategy Mode = EAllocStrategy::BestFit);

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
		NODISCARD int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*), EAllocStrategy Mode = EAllocStrategy::BestFit);

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
		NODISCARD void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*), EAllocStrategy Mode = EAllocStrategy::BestFit);

		/**
		* Frees the chunk where Allocation is placed.
		* Generates errors based on EFailureMode enum value.
		* 
		* @param Allocation Pointer to the memory to be freed.
		* 
		* @param Size Size of the allocation. Currently, used only for debugging.
		*/
		virtual void Free(void* Allocation, uint64 Size);

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
		int32 TryFree(void* Allocation, uint64 Size);

		/**
		* Frees the chunk where Allocation is placed.
		* It doesn't perform ANY error checking and will most likely unreportedly crash the engine if ANY errors occur.
		*
		* @param Allocation Pointer to the memory to be freed.
		*
		* @param Size Size of the allocation. Currently, used only for debugging.
		*/
		void FreeUnsafe(void* Allocation, uint64 Size);

		/**
		* 
		* 
		* @returns 
		*/
		void FreeAll();

		/**
		* 
		* 
		* @returns 
		*/
		int32 TryFreeAll();

		/**
		* 
		*/
		void FreeAllUnsafe();

		/**
		* 
		*/
		virtual void GarbageCollect() override;

		void AllocateNewPage(uint64 ChunksCount, uint64 ChunkSize);

	/* Getters & Setters */
	public:
		/**
		* Returns the total size of the used chunks.
		* Might not be accurate with the requested allocations' sizes.
		*/
		uint64 GetAllocatedSize() const;

		/**
		* Returns the debug tag of the arena.
		*/
		virtual const TChar* GetDebugName() const override;

		/**
		* Returns the total size of the free chunks.
		*/
		uint64 GetFreeSize() const;

		/**
		* Returns the total size of all chunks (free or not).
		*/
		uint64 GetTotalSize() const;

		FORCEINLINE const APoolArenaSpecification& GetSpecification() const { return m_Specification; }

	private:
		APoolArenaSpecification m_Specification;

		TVector<APage*> m_Pages;
	
	/* Friends */
	private:
		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);

		template<typename T>
		friend class TSharedPtr;
	};

}