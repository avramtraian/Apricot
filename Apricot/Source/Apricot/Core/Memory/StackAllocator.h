// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Allocator.h"

namespace Apricot {

	class APRICOT_API AStackAllocator : public AAllocatorBase
	{
	public:
		AStackAllocator();

		AStackAllocator(const AStackAllocator&) = delete;
		AStackAllocator& operator=(const AStackAllocator&) = delete;

		AStackAllocator(AStackAllocator&& Other) noexcept;
		AStackAllocator& operator=(AStackAllocator&& Other) noexcept;

		virtual ~AStackAllocator() override;

	public:
		static void Create(AStackAllocator* Allocator, uint64 TotalSize, void* Memory);
		static void Destroy(AStackAllocator* Allocator);

		virtual void* Allocate(uint64 Size, EAllocTag Tag = EAllocTag::Unknown);
		virtual void Free(void* Memory, uint64 Size, EAllocTag Tag = EAllocTag::Unknown);
		void Free(uint64 Size, EAllocTag Tag = EAllocTag::Unknown);

		void FreeAll();

		static EAllocatorType GetStaticType() { return EAllocatorType::Stack; }

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