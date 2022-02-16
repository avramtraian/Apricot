// Part of Apricot Engine. 2022-2022.

#pragma once

#include "ApricotMemory.h"

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
		bool8 bShouldGrow = false;

		/**
		* 
		*/
		bool8 bAllowAlignment = false;
	};

	/**
	* C++ Core Engine Architecture
	* 
	* Stack Arena implementation
	*/
	class APRICOT_API AStackArena : public AMemoryArena
	{
	/* Constructors & Deconstructor */
	private:
		AStackArena();
		virtual ~AStackArena() override = default;

		AStackArena(const AStackArena&) = delete;
		AStackArena(AStackArena&&) = delete;
		AStackArena& operator=(const AStackArena&) = delete;
		AStackArena& operator=(AStackArena&&) = delete;

	/* Typedefs */
	private:
		struct APage
		{
			/**
			* 
			*/
			void* MemoryBlock = nullptr;

			/**
			* 
			*/
			uint64 Size = 0;
			
			/**
			* 
			*/
			uint64 Allocated = 0;
		};

	/* API interface */
	public:
		/**
		*
		*/
		NODISCARD virtual void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*)) override;

		/**
		*
		*/
		NODISCARD virtual int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*)) override;

		/**
		*
		*/
		NODISCARD virtual void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*)) override;

		/**
		*
		*/
		virtual void Free(void* Allocation, uint64 Size) override;

		/**
		*
		*/
		virtual int32 TryFree(void* Allocation, uint64 Size) override;

		/**
		*
		*/
		virtual void FreeUnsafe(void* Allocation, uint64 Size) override;

		/**
		*
		*/
		virtual void FreeAll() override;

		/**
		*
		*/
		virtual int32 TryFreeAll() override;

		/**
		*
		*/
		virtual void FreeAllUnsafe() override;

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
		virtual uint64 GetTotalSize() const override;

		/**
		*
		*/
		virtual uint64 GetAllocatedSize() const override;

		/**
		*
		*/
		virtual uint64 GetFreeSize() const override;

		/**
		*
		*/
		virtual const TChar* GetDebugName() const override;

		/**
		* Returns the arena's specification.
		*/
		FORCEINLINE const AStackArenaSpecification& GetSpecification() const { return m_Specification; }

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

	/* Friends */
	private:
		friend APRICOT_API uint64 GetStackArenaMemoryRequirementEx(const AStackArenaSpecification&);
		friend APRICOT_API AStackArena* CreateStackArenaEx(const AStackArenaSpecification&);
		friend APRICOT_API void DestroyStackArena(AStackArena*);

		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};

	/**
	* 
	*/
	APRICOT_API uint64 GetStackArenaMemoryRequirementEx(const AStackArenaSpecification&);

	/**
	* 
	*/
	NODISCARD APRICOT_API AStackArena* CreateStackArenaEx(const AStackArenaSpecification&);

	/**
	* 
	*/
	APRICOT_API void DestroyStackArena(AStackArena* Arena);

}