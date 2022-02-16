// Part of Apricot Engine. 2022-2022.

#pragma once

#include "ApricotMemory.h"

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
		* Tells the arena if it can allocate new pages when it gets out of memory.
		* If this is false, and the arena runs out of memory, an 'error' will be issued.
		*/
		bool8 bShouldGrow = true;
	};
	


	/**
	* C++ Core Engine Architecture
	* 
	* Linear Arena implementation
	*/
	class APRICOT_API ALinearArena : public AMemoryArena
	{
	/* Constructors & Deconstructor */
	private:
		ALinearArena();
		virtual ~ALinearArena() override = default;

		ALinearArena(const ALinearArena&) = delete;
		ALinearArena(ALinearArena&&) = delete;
		ALinearArena& operator=(const ALinearArena&) = delete;
		ALinearArena& operator=(ALinearArena&&) = delete;

	/* Typedefs */
	private:
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
		NODISCARD virtual void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*)) override;

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
		NODISCARD virtual int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*)) override;

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
		NODISCARD virtual void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*)) override;

		/**
		* Inheritance artifact.
		* Calling this will issue an 'error'.
		* 
		* None of the parameters matters.
		*/
		virtual void Free(void* Allocation, uint64 Size) override;

		/**
		* Inheritance artifact.
		* Calling this will always return as an error.
		* 
		* None of the parameters matters.
		*/
		virtual int32 TryFree(void* Allocation, uint64 Size) override;

		/**
		* Inheritance artifact.
		* Calling this will do nothing.
		* 
		* None of the parameters matters.
		*/
		virtual void FreeUnsafe(void* Allocation, uint64 Size) override;

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		*/
		virtual void FreeAll() override;

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		* 
		* @returns A flag specifying if any errors were encountered. A simple 'if' statement will check for any error flags.
		*/
		virtual int32 TryFreeAll() override;

		/**
		* This will free all pages, and will not delete any of them.
		* It will always succeed.
		*/
		virtual void FreeAllUnsafe() override;

		/**
		* Deletes all the unused pages. It cannot delete the specification pages, because they aren't allocated individually.
		*/
		virtual void GarbageCollect() override;

	/* Getters & Setters */
	public:
		/**
		* Returns the total size of all pages.
		*/
		virtual uint64 GetTotalSize() const override;

		/**
		* Returns the total allocated size from each page.
		* Might not be accurate with the requested allocations' sizes.
		*/
		virtual uint64 GetAllocatedSize() const override;

		/**
		* Returns the available size. Remember that this memory might not be contiguous, so this is not as the biggest size
		*	that can be allocated without having to allocate a new page.
		*/
		virtual uint64 GetFreeSize() const override;

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
		friend APRICOT_API uint64 GetLinearArenaMemoryRequirementEx(const ALinearArenaSpecification&);
		friend APRICOT_API ALinearArena* CreateLinearArenaEx(const ALinearArenaSpecification&);
		friend APRICOT_API void DestroyLinearArena(ALinearArena*);

		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};

	/**
	* Gets the linear arena memory requirement. Called internally by the 'CreateLinearArenaEx'.
	* Calculates how much memory should be allocated to hold the arena's metadata, pages' metadata and the blocks themselves.
	* 
	* @param Specification Linear Arena's specification.
	* 
	* @returns Memory requirement.
	*/
	NODISCARD APRICOT_API uint64 GetLinearArenaMemoryRequirementEx(const ALinearArenaSpecification& Specification);

	/**
	* Wrapper for 'GetLinearArenaMemoryRequirementEx'.
	*/
	NODISCARD APRICOT_API uint64 GetLinearArenaMemoryRequirement(uint64 SizeBytes);

	/**
	* Wrapper for 'GetLinearArenaMemoryRequirementEx'.
	*/
	NODISCARD APRICOT_API uint64 GetLinearArenaMemoryRequirement(uint64 PagesCount, uint64* PageSizes);

	/**
	* Allocates & Creates a Linear Arena.
	* 
	* @param Specification Linear Arena's specification.
	* 
	* @returns Pointer to the newly created arena.
	*/
	NODISCARD APRICOT_API ALinearArena* CreateLinearArenaEx(const ALinearArenaSpecification& Specification);

	/**
	* Wrapper for 'CreateLinearArenaEx'.
	*/
	NODISCARD APRICOT_API ALinearArena* CreateLinearArena(uint64 SizeBytes, bool8 bShouldGrow, void* Memory = nullptr);

	/**
	* Wrapper for 'CreateLinearArenaEx'.
	*/
	NODISCARD APRICOT_API ALinearArena* CreateLinearArena(uint64 PagesCount, uint64* PageSizes, bool8 bShouldGrow, void* Memory = nullptr);

	/**
	* Destroys a Linear Arena.
	*/
	APRICOT_API void DestroyLinearArena(ALinearArena* Arena);
	
}