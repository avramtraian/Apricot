// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"

#include "Apricot/Containers/Format.h"

namespace Apricot {

	enum class Log : uint8
	{
		Fatal = 0,
		Error = 1,
		Warn  = 2,
		Info  = 3,
		Debug = 4,
		Trace = 5
	};

	class APRICOT_API Logger
	{
	public:
		static void Init();
		static void Destroy();

		static void Write(Log type, const char8* message);
		static void WriteError(Log type, const char8* message);

		template<typename... Args>
		static void Write(Log type, const char8* message, Args&&... args)
		{
			Format(message, s_Buffer, sizeof(s_Buffer), std::forward<Args>(args)...);
			Write(type, s_Buffer);
		}

		template<typename... Args>
		static void WriteError(Log type, const char8* message, Args&&... args)
		{
			Format(message, s_Buffer, sizeof(s_Buffer), std::forward<Args>(args)...);
			WriteError(type, s_Buffer);
		}

	private:
		static char8 s_Buffer[32000];
	};

}

/*
* FATAL - The engine will be forced to shutdown (Severity 0)
*/
#ifdef AE_ENABLE_LOG_FATAL
	#define AE_CORE_FATAL(...) Apricot::Logger::WriteError(Apricot::Log::Fatal, __VA_ARGS__)
#else
	#define AE_CORE_FATAL(...) 
#endif

/*
* ERROR - The engine might recover (Severity 1)
*/
#ifdef AE_ENABLE_LOG_ERROR
	#define AE_CORE_ERROR(...) Apricot::Logger::WriteError(Apricot::Log::Error, __VA_ARGS__)
#else
	#define AE_CORE_ERROR(...) 
#endif

/*
* WARN - Something suboptimal happened (Severity 2)
*/
#ifdef AE_ENABLE_LOG_WARN
	#define AE_CORE_WARN(...) Apricot::Logger::Write(Apricot::Log::Warn, __VA_ARGS__)
#else
	#define AE_CORE_WARN(...) 
#endif

/*
* INFO - Just information about various processes (Severity 3)
*/
#ifdef AE_ENABLE_LOG_INFO
	#define AE_CORE_INFO(...) Apricot::Logger::Write(Apricot::Log::Info, __VA_ARGS__)
#else
	#define AE_CORE_INFO(...) 
#endif

/*
* DEBUG - Debug information (Severity 4)
*/
#ifdef AE_ENABLE_LOG_DEBUG
	#define AE_CORE_DEBUG(...) Apricot::Logger::Write(Apricot::Log::Debug, __VA_ARGS__)
#else
	#define AE_CORE_DEBUG(...) 
#endif

/*
* TRACE - Very detailed information about everything is going on internally (Severity 5)
*/
#ifdef AE_ENABLE_LOG_TRACE
	#define AE_CORE_TRACE(...) Apricot::Logger::Write(Apricot::Log::Trace, __VA_ARGS__)
#else
	#define AE_CORE_TRACE(...) 
#endif