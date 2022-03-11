// Part of Apricot Engine. 2022-2022.
// Submodule: Profiling

#include "aepch.h"
#include "MemoryProfiler.h"

#include "Apricot/Core/Memory/ApricotMemory.h"

namespace Apricot {

	APRICOT_API extern AMemoryProfiler* GMemoryProfiler = nullptr;

	AMemoryProfiler::AMemoryProfiler()
	{
	}

	void AMemoryProfiler::Init()
	{
		if (GMemoryProfiler)
		{
			return;
		}

		GMemoryProfiler = (AMemoryProfiler*)GMalloc->Alloc(sizeof(AMemoryProfiler));
		MemConstruct<AMemoryProfiler>(GMemoryProfiler);
	}

	void AMemoryProfiler::Destroy()
	{
		if (!GMemoryProfiler)
		{
			return;
		}

		GMemoryProfiler->~AMemoryProfiler();
		GMalloc->Free(GMemoryProfiler, sizeof(AMemoryProfiler));
		GMemoryProfiler = nullptr;
	}

	void AMemoryProfiler::SubmitHeapAllocation(uint64 size, uint64 alignment)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		
		AE_CORE_TRACE(TEXT("MemoryProfiler - Heap Allocation Requested: Size '{}', Alignment '{}'"), size, alignment);

#endif // AE_ENABLE_MEMORY_TRACE
	}

	void AMemoryProfiler::SubmitHeapDeallocation(void* block, uint64 size)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		
		AE_CORE_TRACE(TEXT("MemoryProfiler - Heap Deallocation Requested: Size '{}'"), size);

#endif // AE_ENABLE_MEMORY_TRACE
	}

}