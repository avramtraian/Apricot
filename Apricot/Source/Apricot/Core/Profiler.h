#pragma once

#include "Base.h"
#include "Time.h"

#ifdef AE_ENABLE_PERFORMANCE_PROFILING
#include <Optick/optick.h>
#endif

namespace Apricot {

	class APRICOT_API PerformanceProfiler
	{
	public:
		static void Init();
		static void Destroy();
	};

	class APRICOT_API ScopedTimer
	{
	public:
		ScopedTimer(const char* name);
		~ScopedTimer();

	public:
		void EndRecording();

	private:
		HTime m_StartingTime;
		String64 m_TimerName;
	};

}

/*
* Performance profiler macros.
*/
#ifdef AE_ENABLE_PERFORMANCE_PROFILING
	#define AE_PERFORMANCE_BEGIN_FRAME(Name) OPTICK_FRAME(Name)
	#define AE_PERFORMANCE_SCOPE(TimerName)  Apricot::ScopedTimer scTimer(TimerName)
	#define AE_PERFORMANCE_FUNCTION()        OPTICK_EVENT()
#else
	#define AE_PERFORMANCE_BEGIN_FRAME(Name) 
	#define AE_PERFORMANCE_SCOPE(TimerName) 
	#define AE_PERFORMANCE_FUNCTION() 
#endif