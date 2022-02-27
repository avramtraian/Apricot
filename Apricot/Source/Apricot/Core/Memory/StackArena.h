// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "ApricotMemory.h"
#include "Apricot/Core/AClass.h"

namespace Apricot {
	
	/**
	* Stack Arena specification
	*/
	struct AStackArenaSpecification
	{
		/**
		* 
		*/
		uint64 PagesCount = 0;

		/**
		* 
		*/
		uint64* PageSizes = nullptr;

		/**
		* 
		*/
		void* ArenaMemory = nullptr;

		/**
		* 
		*/
		uint64 ArenaMemoryOffset = 0;

		/**
		* 
		*/
		bool bShouldGrow = false;

		/**
		* 
		*/
		bool bAllowAlignment = false;

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
	* Stack Arena implementation
	*/
	class APRICOT_API AStackArena : public AMemoryArena
	{
		ACLASS_CORE()

	public:
		NODISCARD static TSharedPtr<AStackArena> Create(const AStackArenaSpecification& Specification);

		NODISCARD static uint64 GetPageMemoryRequirement(uint64 PageSize);

		NODISCARD static uint64 GetMemoryRequirement(const AStackArenaSpecification& Specification);

	/* Constructors & Deconstructor */
	private:
		AStackArena(const AStackArenaSpecification& Specification);
		virtual ~AStackArena() override;

		AStackArena(const AStackArena&) = delete;
		AStackArena(AStackArena&&) = delete;
		AStackArena& operator=(const AStackArena&) = delete;
		AStackArena& operator=(AStackArena&&) = delete;

	/* Typedefs */
	public:
		struct APage
		{
			/**
			* 
			*/
			void* MemoryBlock = nullptr;

			/**
			* 
			*/
			uint64 SizeBytes = 0;
			
			/**
			* 
			*/
			uint64 AllocatedBytes = 0;
		};

		struct AAlignmentInfo
		{
			uint16 AlignmentOffset = 0;
		};

	/* API interface */
	public:
		/**
		*
		*/
		NODISCARD void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*));

		/**
		*
		*/
		NODISCARD int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*));

		/**
		*
		*/
		NODISCARD void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*));

		/**
		*
		*/
		void Free(void* Allocation, uint64 Size);

		/**
		*
		*/
		int32 TryFree(void* Allocation, uint64 Size);

		/**
		*
		*/
		void FreeUnsafe(void* Allocation, uint64 Size);

		/**
		*
		*/
		void FreeAll();

		/**
		*
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

		/**
		*
		*/
		void Pop(uint64 Size);

		/**
		*
		*/
		int32 TryPop(uint64 Size);

		/**
		*
		*/
		void PopUnsafe(uint64 Size);
	
	/* Getters & Setters */
	public:
		/**
		*
		*/
		uint64 GetTotalSize() const;

		/**
		*
		*/
		uint64 GetAllocatedSize() const;

		/**
		*
		*/
		uint64 GetFreeSize() const;

		/**
		*
		*/
		virtual const TChar* GetDebugName() const override;

		/**
		* Returns the arena's specification.
		*/
		FORCEINLINE const AStackArenaSpecification& GetSpecification() const { return m_Specification; }

	private:
		/**
		* 
		* 
		* @param PageSize 
		* 
		* @returns 
		*/
		APage* AllocateNewPage(uint64 PageSize);

		/**
		* 
		* 
		* @param RequestedAllocationSize 
		* 
		* @returns 
		*/
		uint64 GetOptimalPageSize(uint64 RequestedAllocationSize);

		/**
		* 
		* 
		* @param Allocation 
		* 
		* @param Size 
		* 
		* @returns 
		*/
		bool8 CouldBeFreed(void* Allocation, uint64 Size) const;

	/* Member variables */
	private:
		/**
		* 
		*/
		AStackArenaSpecification m_Specification;

		/**
		* 
		*/
		TVector<APage*> m_Pages;

		/**
		* 
		*/
		uint64 m_CurrentPage;

	/* Friends */
	private:
		template<typename T, typename... Args>
		friend constexpr FORCEINLINE T* MemConstruct(void*, Args&&...);
	};

}