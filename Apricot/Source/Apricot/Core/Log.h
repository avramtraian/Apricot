#pragma once

#include "Base.h"

namespace Apricot {
	
	/*
	* Fatal -> Red backgroung
	* Error -> Red text
	* Warn	-> Yellow text
	* Info	-> Green text
	* Debug -> Purple text
	* Trace -> Grey text
	*/
	enum class Log : uint8
	{
		Fatal, Error, Warn, Info, Debug, Trace,
		MaxEnum
	};

	/*
	* The class in charge of every logging operation
	*/
	class APRICOT_API Logger
	{
	public:
		static void Init();
		static void Destroy();

		/*
		* Prints a message to the console
		* 
		* @param type Determine the color of the message in the console
		* @param message The message to be printed
		*/
		static void LogCoreMessage(Log type, const char* message);

		/*
		* Prints a message to the console
		* It formats the variadic arguments via THArgumentString<T> struct
		* 
		* @param type Determine the color of the message in the console
		* @param message The message to be printed
		*/
		template<typename... Args>
		static void LogCoreMessage(Log type, const char* message, Args&&... args)
		{
			// TODO: Implement!
		}

	private:
		static char* s_MessageBuffer;
		static uint64 s_MessageBufferSize;
	};

}

/*
* FATAL - The engine will be forced to shutdown (Severity 0)
*/
#ifdef AE_ENABLE_LOG_FATAL
	#define AE_CORE_FATAL(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Fatal, __VA_ARGS__)
#else
	#define AE_CORE_FATAL(...) 
#endif

/*
* ERROR - The engine might recover (Severity 1)
*/
#ifdef AE_ENABLE_LOG_ERROR
	#define AE_CORE_ERROR(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Error, __VA_ARGS__)
#else
	#define AE_CORE_ERROR(...) 
#endif

/*
* WARN - Something suboptimal happened (Severity 2)
*/
#ifdef AE_ENABLE_LOG_WARN
	#define AE_CORE_WARN(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Warn, __VA_ARGS__)
#else
	#define AE_CORE_WARN(...) 
#endif

/*
* INFO - Just information about various processes (Severity 3)
*/
#ifdef AE_ENABLE_LOG_INFO
	#define AE_CORE_INFO(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Info, __VA_ARGS__)
#else
	#define AE_CORE_INFO(...) 
#endif

/*
* DEBUG - Debug information (Severity 4)
*/
#ifdef AE_ENABLE_LOG_DEBUG
	#define AE_CORE_DEBUG(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Debug, __VA_ARGS__)
#else
	#define AE_CORE_DEBUG(...) 
#endif

/*
* TRACE - Very detailed information about everything is going on internally (Severity 5)
*/
#ifdef AE_ENABLE_LOG_TRACE
	#define AE_CORE_TRACE(...) Apricot::Logger::LogCoreMessage(Apricot::Log::Trace, __VA_ARGS__)
#else
	#define AE_CORE_TRACE(...) 
#endif