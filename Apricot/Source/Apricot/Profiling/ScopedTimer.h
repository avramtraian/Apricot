// Part of Apricot Engine. 2022-2022.
// Submodule: Profiling

#pragma once

#include "Apricot/Core/Time.h"
#include "Apricot/Core/Platform.h"
#include "Apricot/Core/Log.h"

namespace Apricot {

	class ScopedTimer
	{
	public:
		ScopedTimer(const TChar* name)
			: m_TimerName(name)
		{
			m_TimerStart = APlatform::GetSystemPerformanceTime();
		}

		~ScopedTimer()
		{
			Stop();
		}

		FORCEINLINE void Stop()
		{
			Time now = APlatform::GetSystemPerformanceTime();
			Time duration = now - m_TimerStart;

			AE_CORE_INFO(TEXT("Timer: '{}' - {}ms"), m_TimerName, duration.Miliseconds());
		}

	private:
		Time m_TimerStart;
		const TChar* m_TimerName;
	};

}