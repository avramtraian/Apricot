#include "aepch.h"
#include "Memory.h"

namespace Apricot { namespace Memory {

	uint64 HMemoryDebugger::AllocationsCount[(uint32)AllocTag::MaxEnum] = {0};
	uint64 HMemoryDebugger::AllocationSizes[(uint32)AllocTag::MaxEnum] = { 0 };

	void HMemoryDebugger::DebugLog()
	{
		
	}

	void HMemoryDebugger::DebugLogAllocationSizes()
	{
		
	}

	void HMemoryDebugger::DebugLogAllocatiosCount()
	{
		
	}

} }