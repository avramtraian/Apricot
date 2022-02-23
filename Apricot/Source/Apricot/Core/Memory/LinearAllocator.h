// Part of Apricot Engine. 2022-2022.

#include "Allocator.h"

#pragma once

namespace Apricot {

	class APRICOT_API ALinearAllocator : public AAllocatorBase
	{
	public:
		ALinearAllocator();

		ALinearAllocator(const ALinearAllocator&) = delete;
		ALinearAllocator& operator=(const ALinearAllocator&) = delete;

		ALinearAllocator(ALinearAllocator&& Other) noexcept;
		ALinearAllocator& operator=(ALinearAllocator&& Other) noexcept;

		virtual ~ALinearAllocator() override;

	public:
		static void Create(ALinearAllocator* Allocator, uint64 TotalSize, void* Memory = nullptr);
		static void Destroy(ALinearAllocator* Allocator);

		virtual void* Allocate(uint64 Size, EAllocTag Tag = EAllocTag::Unknown) override;
		virtual void Free(void* Memory, uint64 Size, EAllocTag Tag = EAllocTag::Unknown) override;

		void FreeAll();

		static EAllocatorType GetStaticType() { return EAllocatorType::Linear; }

	private:
		/*
		* 
		*/
		void* m_Memory;

		/*
		*
		*/
		uint64 m_TotalSize;

		/*
		*
		*/
		uint64 m_Allocated;

		/*
		*
		*/
		bool8 m_OwnsMemory;
	};

}