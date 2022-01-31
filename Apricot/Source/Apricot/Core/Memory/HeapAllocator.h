// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Allocator.h"

namespace Apricot {

	class APRICOT_API AHeapAllocator : public AAllocatorBase
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag Tag) override;
		virtual void Free(void* Memory, uint64 Size, EAllocTag Tag) override;

		static EAllocatorType GetStaticType() { return EAllocatorType::Heap; }
	};

	APRICOT_API extern AHeapAllocator* GHeapAllocator;

}