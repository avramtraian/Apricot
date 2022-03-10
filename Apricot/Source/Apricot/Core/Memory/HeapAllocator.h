// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "ApricotAllocator.h"
#include "Apricot/Core/UUID.h"

namespace Apricot {

	class APRICOT_API HeapAllocator
	{
	public:
		static HeapAllocator* GetDefault();

	public:
		UUID GetUUID() const { return m_UUID; }
		static EAllocatorType GetStaticType() { return EAllocatorType::Heap; }

	public:
		void* Alloc(uint64 size, EAllocatorHint hint);
		void* Realloc(void* oldBlock, uint64 size, EAllocatorHint hint);
		void* ExpandBlock(void* block, uint64 newSize, EAllocatorHint hint);

		void Free(void* block, uint64 size, EAllocatorHint hint);

	private:
		UUID m_UUID;
	};

	extern APRICOT_API HeapAllocator* GHeapAllocator;

}