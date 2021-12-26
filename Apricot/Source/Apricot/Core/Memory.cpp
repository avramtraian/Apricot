#include "aepch.h"
#include "Memory.h"

namespace Apricot { namespace Memory {

	TArray<uint64, (uint64)(uint32)AllocTag::MaxEnum> HMemoryDebugger::AllocationSizes;
	TArray<uint64, (uint64)(uint32)AllocTag::MaxEnum> HMemoryDebugger::AllocationsCount;

	static const char* s_AllocationNames[] =
	{
		"NONE            ",
		"GENERAL         ",
		"CORE_SYSTEMS    ",
		"ALLOCATOR_LINEAR",
		"ALLOCATOR_STACK ",
		"STRING          ",
		"VECTOR          "
	};

	void HMemoryDebugger::DebugLog()
	{
		DebugLogAllocationSizes();
		DebugLogAllocatiosCount();
	}

	void HMemoryDebugger::DebugLogAllocationSizes()
	{
		AE_CORE_DEBUG("Memory Allocation Sizes:");
		for (uint64 index = 0; index < AllocationSizes.Size(); index++)
		{
			AE_CORE_DEBUG("    - {0}: {1}{2}", s_AllocationNames[index], AllocationSizes[index], "B");
		}
	}

	void HMemoryDebugger::DebugLogAllocatiosCount()
	{
		AE_CORE_DEBUG("Memory Allocations Count:");
		for (uint64 index = 0; index < AllocationsCount.Size(); index++)
		{
			AE_CORE_DEBUG("    - {0}: {1}{2}", s_AllocationNames[index], AllocationsCount[index]);
		}
	}

} }