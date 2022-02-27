// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "ApricotMemory.h"
#include "Apricot/Core/AClass.h"

namespace Apricot {

	/**
	* ALinearArena Specification
	* 
	* These are the initial arena's specifications. Might not be up-to-date.
	*/
	struct ALinearArenaSpecification
	{
		/**
		* Count of initial pages. These are allocated at the same time as the arena itself.
		*/
		uint64 PagesCount = 0;

		/**
		* Array of sizes for the initial pages. Must be the same size as 'PagesCount'.
		* Might be invalid after the arena creation. So this is a temporary parameter.
		*/
		uint64* PageSizes = nullptr;

		/**
		* Pointer to a memory block that will be used to allocate memory for the arena. Pointer should
		*	be valid on the entire arena's lifetime. If this is nullptr, the memory will be allocated from the global heap.
		*/
		void* ArenaMemory = nullptr;

		/**
		* 
		*/
		uint64 ArenaMemoryOffset = 0;

		/**
		* Tells the arena if it can allocate new pages when it gets out of memory.
		* If this is false, and the arena runs out of memory, an 'error' will be issued.
		*/
		bool bShouldGrow = true;

		/**
		* 
		*/
		bool bUseArenaMemoryAlways = false;

		/**
		* 
		*/
		bool bBulkAllocateSpecPages = true;
	};
	


	/**
	* C++ Core Engine Architecture
	* 
	* Linear Arena implementation
	*/
	class APRICOT_API ALinearArena : public AMemoryArena
	{
		ACLASS_CORE()

	public:
		NODISCARD static uint64 GetPageMemoryRequirement(uint64 PageSizeBytes);

		NODISCARD static uint64 GetMemoryRequirement(const ALinearArenaSpecification& Specification);

		NODISCARD static TSharedPtr<ALinearArena> Create(const ALinearArenaSpecification& Specification);

	/* Constructors & Deconstructor */
	private:
		ALinearArena(const ALinearArenaSpecification& Specification);
		virtual ~ALinearArena() override;

		ALinearArena(const ALinearArena&) = delete;
		ALinearArena(ALinearArena&&) = delete;
		ALinearArena& operator=(const ALinearArena&) = delete;
		ALinearArena& operator=(ALinearArena&&) = delete;

	/* Typedefs */
	public:
		struct APage
		{
			/**
			* Pointer to the page's memory.
			*/
			void* MemoryBlock = nullptr;

			/**
			* The size (in bytes) of the memory block.
			*/
			uint64 SizeBytes = AE_INVALID_MEMSIZE;

			/**
			* The allocated memory (in bytes).
			*/
			uint64 AllocatedBytes = AE_INVALID_MEMSIZE;
		};

	/* API interface */
	public:
		/**
		* Allocates a block of memory.
		* Issues errors based on the value of EFailureMode.
		* 
		* @param Size The size of the allocation.
		* 
		* @param Alignment The alignment of the memory block. Might cause the arena to allocate more than 'Size'.
		*			Default value is sizeof(void*), which, in case of allocating structs bigger than sizeof(void) bytes, is optimal for most platforms.
		* 
		* @returns Pointer to the aligned memory block. In case of failure, it will return nullptr.
		*/
		NODISCARD void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*));

		/**
		* Allocates a block of memory.
		* Same as 'Alloc', but it returns a flag indicating failure or success.
		*
		* @param Size The size of the allocation.
		* 
		* @param OutPointer Out parameter, holds the 'returned' pointer. Must NOT be nullptr.
		*
		* @param Alignment The alignment of the memory block. Might cause the arena to allocate more than 'Size'.
		*			Default value is sizeof(void*), which, in case of allocating structs bigger than sizeof(void) bytes, is optimal for most platforms.
		* 
		* @returns EEAllocErrors flag indicating failure or success. Possible error return codes: AE_ALLOC_BAD_ARENA, AE_ALLOC_INVALID_SIZE, AE_ALLOC_INVALID_PARAM,
		*				AE_ALLOC_BAD_ALIGNMENT, AE_ALLOC_OUT_OF_MEMORY.
		*/
		NODISCARD int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*));

		/**
		* Allocates a block of memory.
		* Does NOT issue ANY errors.
		*
		* @param Size The size of the allocation.
		*
		* @param Alignment The alignment of the memory block. Might cause the arena to allocate more than 'Size'.
		*			Default value is sizeof(void*), which, in case of allocating structs bigger than sizeof(void) bytes, is optimal for most platforms.
		*
		* @returns Pointer to the aligned memory block. It will return nullptr ONLY when the arena is fully allocated and 'bShouldGrow' is false.
		*/
		NODISCARD void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*));

		/**
		* Inheritance artifact.
		* Calling this will issue an 'error'.
		* 
		* None of the parameters matters.
		*/
		void Free(void* Allocation, uint64 Size);

		/**
		* Inheritance artifact.
		* Calling this will always return as an error.
		* 
		* None of the parameters matters.
		*/
		int32 TryFree(void* Allocation, uint64 Size);

		/**
		* Inheritance artifact.
		* Calling this will do nothing.
		* 
		* None of the parameters matters.
		*/
		void FreeUnsafe(void* Allocation, uint64 Size);

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		*/
		void FreeAll();

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		* 
		* @returns A flag specifying if any errors were encountered. A simple 'if' statement will check for any error flags.
		*/
		int32 TryFreeAll();

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		*/
		void FreeAllUnsafe();

		/**
		* Deletes all the unused pages. It cannot delete the specification pages, because they aren't allocated individually.
		*/
		virtual void GarbageCollect() override;

	/* Getters & Setters */
	public:
		/**
		* Returns the total size of all pages.
		*/
		uint64 GetTotalSize() const;

		/**
		* Returns the total allocated size from each page.
		* Might not be accurate with the requested allocations' sizes.
		*/
		uint64 GetAllocatedSize() const;

		/**
		* Returns the available size. Remember that this memory might not be contiguous, so this is not as the biggest size
		*	that can be allocated without having to allocate a new page.
		*/
		uint64 GetFreeSize() const;

		/**
		* Returns the debug tag of the arena.
		*/
		virtual const TChar* GetDebugName() const override;

		/**
		* Returns the arena's specification.
		*/
		FORCEINLINE const ALinearArenaSpecification& GetSpecification() const { return m_Specification; }

	private:
		/**
		* Calculates the size of the next allocated page. Takes into account the requested allocation size and the sizes of the existing pages.
		* 
		* @param RequestedAllocationSize Allocation size.
		* 
		* @returns The size of the new page.
		*/
		uint64 GetOptimalPageSize(uint64 RequestedAllocationSize) const;

		/**
		* Allocates enough memory for a page metadata and the its block.
		* Also, it pushes the page pointer to the arena's vector.
		* 
		* @param PageSize The size of the new page.
		* 
		* @returns Pointer to the newly created page.
		*/
		APage* AllocateNewPage(uint64 PageSize);

	/* Member variables */
	private:
		/**
		* Vector of all available arena's pages (pointers to them).
		*/
		TVector<APage*> m_Pages;

		/**
		* Index of the page that is filling.
		*/
		uint64 m_CurrentPage;

		/**
		* Arena's specification. Can't be modified after the creation.
		*/
		ALinearArenaSpecification m_Specification;

	/* Friends */
	private:
		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};
	
}