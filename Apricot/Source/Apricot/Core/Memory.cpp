#include "aepch.h"
#include "Memory.h"

namespace Apricot { namespace Memory {

	uint64 HMemoryDebugger::TotalDeallocations[];
	uint64 HMemoryDebugger::TotalAllocations[];
	uint64 HMemoryDebugger::TotalFreedMemory[];
	uint64 HMemoryDebugger::TotalAllocatedMemory[];

	static const char8* s_AllocationNames[] =
	{
		"NONE            ",
		"GENERAL         ",
		"CORE_SYSTEMS    ",
		"OPTICK          ",
		"ALLOCATOR_LINEAR",
		"ALLOCATOR_STACK ",
		"STRING          ",
		"VECTOR          "
	};

	static const char8* s_MemoryUnitNames[] =
	{
		"B",
		"KB",
		"MB",
		"GB"
	};

	namespace Utils {

		static float64 GetUnit(uint64 bytes)
		{
			if (bytes >= AE_GIGABYTES(1))
				return (float64)bytes / (float64)(AE_GIGABYTES(1));
			else if (bytes >= AE_MEGABYTES(1))
				return (float64)bytes / (float64)(AE_MEGABYTES(1));
			else if (bytes >= AE_KILOBYTES(1))
				return (float64)bytes / (float64)(AE_KILOBYTES(1));
			else
				return (float64)bytes;
		}

		static const char8* GetUnitName(uint64 bytes)
		{
			if (bytes >= AE_GIGABYTES(1))
				return s_MemoryUnitNames[3];
			else if (bytes >= AE_MEGABYTES(1))
				return s_MemoryUnitNames[2];
			else if (bytes >= AE_KILOBYTES(1))
				return s_MemoryUnitNames[1];
			else
				return s_MemoryUnitNames[0];
		}

	}

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
				AE_CORE_DEBUG("      TotalAllocatedMemory:    {}{}", Utils::GetUnit(TotalAllocatedMemory[index]), Utils::GetUnitName(TotalAllocatedMemory[index]));
			}

			if (bTotalFreed)
			{
				AE_CORE_DEBUG("      TotalFreedMemory:        {}{}", Utils::GetUnit(TotalFreedMemory[index]), Utils::GetUnitName(TotalAllocatedMemory[index]));
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
				AE_CORE_DEBUG("      ActiveAllocatedMemory:   {}{}", Utils::GetUnit(TotalAllocatedMemory[index] - TotalFreedMemory[index]), Utils::GetUnitName(TotalAllocatedMemory[index] - TotalFreedMemory[index]));
			}
			if (bActiveAllocations)
			{
				AE_CORE_DEBUG("      ActiveAllocations:       {}", TotalAllocations[index] - TotalDeallocations[index]);
			}
		}
		AE_CORE_DEBUG("##--------------------------------##");
	}

} }