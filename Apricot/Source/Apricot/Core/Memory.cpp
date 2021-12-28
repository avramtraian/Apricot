#include "aepch.h"
#include "Memory.h"

namespace Apricot { namespace Memory {

	uint64 HMemoryDebugger::TotalDeallocations[];
	uint64 HMemoryDebugger::TotalAllocations[];
	uint64 HMemoryDebugger::TotalFreedMemory[];
	uint64 HMemoryDebugger::TotalAllocatedMemory[];

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

	void HMemoryDebugger::DebugLog(
		bool bTotalAllocated   /*= true*/, bool bTotalFreed         /*= true*/,
		bool bTotalAllocations /*= true*/, bool bTotalDeallocations /*= true*/,
		bool bActiveAllocated  /*= true*/, bool bActiveAllocations  /*= true*/
	)
	{
		AE_CORE_DEBUG("##--- MEMORY DEBUG INFORMATION ---##");
		for (uint64 index = 0; index < (uint64)AllocTag::MaxEnum; index++)
		{
			AE_CORE_DEBUG("  * {}", s_AllocationNames[index]);

			if (bTotalAllocated)
			{
				AE_CORE_DEBUG("      TotalAllocatedMemory:    {}{}", TotalAllocatedMemory[index], "B");
			}

			if (bTotalFreed)
			{
				AE_CORE_DEBUG("      TotalFreedMemory:        {}{}", TotalFreedMemory[index], "B");
			}

			AE_CORE_DEBUG("      -----------------------------------");

			if (bTotalAllocations)
			{
				AE_CORE_DEBUG("      TotalAllocations:        {}", TotalAllocations[index]);
			}
			if (bTotalDeallocations)
			{
				AE_CORE_DEBUG("      TotalDeallocations:      {}", TotalDeallocations[index]);
			}

			AE_CORE_DEBUG("      -----------------------------------");

			if (bActiveAllocated)
			{
				AE_CORE_DEBUG("      ActiveAllocatedMemory:   {}{}", TotalAllocatedMemory[index] - TotalFreedMemory[index], "B");
			}
			if (bActiveAllocations)
			{
				AE_CORE_DEBUG("      ActiveAllocations:       {}", TotalAllocations[index] - TotalDeallocations[index]);
			}
		}
		AE_CORE_DEBUG("##--------------------------------##");
	}

} }