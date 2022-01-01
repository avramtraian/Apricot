// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Profiler.h"

#include "Memory.h"
#include "Platform.h"

namespace Apricot {

	void PerformanceProfiler::Init()
	{
#ifdef AE_ENABLE_PERFORMANCE_PROFILING
		OPTICK_SET_MEMORY_ALLOCATOR(
			[](uint64 size) -> void* { return Memory::Allocate(size, Memory::AllocTag::Optick); },
			[](void* block) -> void  { Memory::Free(block, 0, Memory::AllocTag::Optick); },
			[]() {}
		);
#endif
	}

	void PerformanceProfiler::Destroy()
	{
#ifdef AE_ENABLE_PERFORMANCE_PROFILING
		OPTICK_SHUTDOWN();
#endif
	}

	ScopedTimer::ScopedTimer(const char* name)
		: m_TimerName(name)
	{
		m_StartingTime = Platform::GetPerformanceTime();
	}

	ScopedTimer::~ScopedTimer()
	{
		EndRecording();
	}

	void ScopedTimer::EndRecording()
	{
		HTime endingTime = Platform::GetPerformanceTime();
		uint64 elapsedNanoseconds = endingTime - m_StartingTime;
		float64 elapsesMiliseconds = (float64)elapsedNanoseconds * 0.000001;

		AE_CORE_TRACE("Timer '{}': {}ms", m_TimerName.c_str(), elapsesMiliseconds);
	}

}