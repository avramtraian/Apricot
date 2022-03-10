// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "HeapAllocator.h"

namespace Apricot {

	extern APRICOT_API HeapAllocator* GHeapAllocator = nullptr;

	HeapAllocator* HeapAllocator::GetDefault()
	{
		return GHeapAllocator;
	}

	void* HeapAllocator::Alloc(uint64 size, EAllocatorHint hint)
	{
		return GMalloc->Alloc(size);
	}

	void* HeapAllocator::Realloc(void* oldBlock, uint64 size, EAllocatorHint hint)
	{
		return GMalloc->Alloc(size);
	}

	void* HeapAllocator::ExpandBlock(void* block, uint64 newSize, EAllocatorHint hint)
	{
		return nullptr;
	}

	void HeapAllocator::Free(void* block, uint64 size, EAllocatorHint hint)
	{
		GMalloc->Free(block, size);
	}

}