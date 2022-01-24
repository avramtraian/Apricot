// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Allocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "DynamicAllocator.h"

namespace Apricot {
	
	void* GlobalAllocator::Allocate(uint64 size, AllocTag reservedTag /* = AllocTag::Unknown */)
	{
		return Memory_Alloc(size, reservedTag);
	}
	void GlobalAllocator::Free(void* memory, uint64 size, AllocTag reservedTag /* = AllocTag::Unknown */)
	{
		Memory_Free(memory, size, reservedTag);
	}
	static GlobalAllocator s_GlobalAllocator;

	GlobalAllocator* GAllocator = &s_GlobalAllocator;

}