// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Allocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "DynamicAllocator.h"

namespace Apricot {
	
	void* AGlobalAllocator::Allocate(uint64 Size, EAllocTag ReservedTag /* = AllocTag::Unknown */)
	{
		return Memory_Alloc(Size, ReservedTag);
	}
	void AGlobalAllocator::Free(void* Memory, uint64 Size, EAllocTag ReservedTag /* = AllocTag::Unknown */)
	{
		Memory_Free(Memory, Size, ReservedTag);
	}
	static AGlobalAllocator s_GlobalAllocator;

	AGlobalAllocator* GAllocator = &s_GlobalAllocator;

}