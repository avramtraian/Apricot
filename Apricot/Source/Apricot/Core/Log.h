// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#include "Apricot/Containers/Format.h"

namespace Apricot {

	enum class ELog : uint8
	{
		Fatal = 0,
		Error = 1,
		Warn  = 2,
		Info  = 3,
		Debug = 4,
		Trace = 5
	};

	class APRICOT_API ALogger
	{
	public:
		static void Init();
		static void Destroy();

		static void Write(ELog Type, const TChar* Message);
		static void WriteError(ELog Type, const TChar* Message);

		template<typename... Args>
		static void Write(ELog Type, const TChar* Message, Args&&... args)
		{
			Format(SBuffer, AE_ARRAY_LENGTH(SBuffer), Message, std::forward<Args>(args)...);
			Write(Type, SBuffer);
		}

		template<typename... Args>
		static void WriteError(ELog Type, const TChar* Message, Args&&... args)
		{
			Format(SBuffer, AE_ARRAY_LENGTH(SBuffer), Message, std::forward<Args>(args)...);
			WriteError(Type, SBuffer);
		}

	private:
		static TChar SBuffer[32000];
	};

}

/*
* FATAL - The engine will be forced to shutdown (Severity 0)
*/
#ifdef AE_ENABLE_LOG_FATAL
	#define AE_CORE_FATAL(...) Apricot::ALogger::WriteError(Apricot::ELog::Fatal, __VA_ARGS__)
#else
	#define AE_CORE_FATAL(...) 
#endif

/*
* ERROR - The engine might recover (Severity 1)
*/
#ifdef AE_ENABLE_LOG_ERROR
	#define AE_CORE_ERROR(...) Apricot::ALogger::WriteError(Apricot::ELog::Error, __VA_ARGS__)
#else
	#define AE_CORE_ERROR(...) 
#endif

/*
* WARN - Something suboptimal happened (Severity 2)
*/
#ifdef AE_ENABLE_LOG_WARN
	#define AE_CORE_WARN(...) Apricot::ALogger::Write(Apricot::ELog::Warn, __VA_ARGS__)
#else
	#define AE_CORE_WARN(...) 
#endif

/*
* INFO - Just information about various processes (Severity 3)
*/
#ifdef AE_ENABLE_LOG_INFO
	#define AE_CORE_INFO(...) Apricot::ALogger::Write(Apricot::ELog::Info, __VA_ARGS__)
#else
	#define AE_CORE_INFO(...) 
#endif

/*
* DEBUG - Debug information (Severity 4)
*/
#ifdef AE_ENABLE_LOG_DEBUG
	#define AE_CORE_DEBUG(...) Apricot::ALogger::Write(Apricot::ELog::Debug, __VA_ARGS__)
#else
	#define AE_CORE_DEBUG(...) 
#endif

/*
* TRACE - Very detailed information about everything is going on internally (Severity 5)
*/
#ifdef AE_ENABLE_LOG_TRACE
	#define AE_CORE_TRACE(...) Apricot::ALogger::Write(Apricot::ELog::Trace, __VA_ARGS__)
#else
	#define AE_CORE_TRACE(...) 
#endif