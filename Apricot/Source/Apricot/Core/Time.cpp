#include "aepch.h"
#include "Time.h"

#include "Platform.h"

namespace Apricot {

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
		HTime l_EndingTime = Platform::GetPerformanceTime();
		uint64 l_ElapsedNanoseconds = l_EndingTime - m_StartingTime;
		float64 l_ElapsesMiliseconds = (float64)l_ElapsedNanoseconds * 0.000001;
		
		AE_CORE_TRACE("Timer '{}': {}ms", m_TimerName.c_str(), l_ElapsesMiliseconds);
	}

}