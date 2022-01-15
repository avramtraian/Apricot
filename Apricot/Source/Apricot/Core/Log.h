// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Config.h"
#include "Base.h"
#include "Allocators.h"

#include "Apricot/Utils/RawStringFormatter.h"

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
	* The class in charge of every logging operation.
	*/
	class APRICOT_API Logger
	{
	public:
		static void Init();
		static void Destroy();

		/*
		* Prints a message to the console.
		* 
		* @param type Determine the color of the message in the console.
		* @param message The message to be printed.
		*/
		static void LogCoreMessage(Log type, const char8* message);

		/*
		* Prints a message to the console.
		* Can format the message
		* 
		* @param type Determine the color of the message in the console.
		* @param message The message to be printed.
		* @param args The argument pack - It will be formatted.
		*/
		template<typename... Args>
		static void LogCoreMessage(Log type, const char8* message, Args&&... args)
		{
			if (!s_bIsConsoleEnabled)
			{
				return;
			}

			// Reset the allocator.
			s_ArgsAllocator.FreeAll();

			// Create the formatters.
			THFormatter<char8>* arguments = (THFormatter<char8>*)s_ArgsAllocator.Allocate(AEC_MAX_LOG_ARGUMENT_COUNT * sizeof(THFormatter<char8>));
			for (uint64 index = 0; index < AEC_MAX_LOG_ARGUMENT_COUNT; index++)
			{
				Memory::PlacementNew<THFormatter<char8>>(arguments + index, &s_ArgsAllocator);
			}

			// Fill the formatters.
			uint64 argumentsCount = 0;
			const char8* formatted = RawStringFormatter::FormatComplex(s_ArgsAllocator, arguments, AEC_MAX_LOG_ARGUMENT_COUNT, argumentsCount, message, std::forward<Args>(args)...);

			// Send the formatted message to the platform layer (indirectly).
			LogCoreMessage(type, formatted);
		}

	private:
		/*
		* The buffer where we store the message before sending it to the platform layer.
		*/
		static char8* s_MessageBuffer;

		/*
		* The size (in bytes) of the message buffer.
		*/
		static uint64 s_MessageBufferSize;

		/*
		* Allocator used for allocating memory for the formatting.
		* It gets reseted every time we start formatting a string.
		*/
		static Memory::LinearAllocator s_ArgsAllocator;

		static bool8 s_bIsConsoleEnabled;
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

#undef EXCLUDE_STRING