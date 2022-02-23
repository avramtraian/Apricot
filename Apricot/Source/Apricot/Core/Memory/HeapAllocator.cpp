#include "aepch.h"
#include "HeapAllocator.h"

namespace Apricot {

	AHeapAllocator* GHeapAllocator = nullptr;

	void AHeapAllocator::Init()
	{
		static AHeapAllocator HeapAllocator;
		GHeapAllocator = &HeapAllocator;
	}

	void AHeapAllocator::Destroy()
	{

	}

	void* AHeapAllocator::Allocate(uint64 Size, EAllocTag Tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		AMemoryDebugger::AllocatedTagged        [(uint16)Tag] += Size;
		AMemoryDebugger::AllocationsCountTagged [(uint16)Tag] += 1;
#endif
		return Memory_Alloc(Size, EAllocSubregion::HeapAllocator);
	}

	void AHeapAllocator::Free(void* Memory, uint64 Size, EAllocTag Tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		AMemoryDebugger::FreedTagged              [(uint16)Tag] += Size;
		AMemoryDebugger::DeallocationsCountTagged [(uint16)Tag] += 1;
#endif
		Memory_Free(Memory, Size, EAllocSubregion::HeapAllocator);
	}
	
}