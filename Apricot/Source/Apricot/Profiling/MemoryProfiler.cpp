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

}