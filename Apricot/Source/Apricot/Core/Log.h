#pragma once

#include "Config.h"
#include "Base.h"
#include "Allocators.h"

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
	* The class in charge of formatting any type.
	* Has strong relationship with the 'THFormatter<T>& operator<<(THFormatter<T>&, const Q&)' function.
	* 
	* @param T The type of string's characters (char8 or char16).
	*/
	template<typename T>
	struct THFormatter
	{
	public:
		/*
		* Internal usage only.
		* You should never instantiate a THFormatter on your own.
		*/
		THFormatter(Memory::LinearAllocator* allocator)
			: Allocator(allocator) {}
	
		/*
		* A thin wrapper function for the allocator's interface.
		* It sets the formatter's Size to the requested memory block size.
		* It's recommended to use this function.
		* 
		* @param size The size (in bytes) of the requested allocation.
		* @returns Pointer to the allocated memory. It is the same as 'Data'.
		*/
		T* Allocate(uint64 size)
		{
			Data = (T*)Allocator->Allocate(size);
			Size = size;
			return Data;
		}

	public:
		/*
		* The allocator used to allocate memory for the string.
		*/
		Memory::LinearAllocator* Allocator = nullptr;
		
		/*
		* The address in the allocator where the string is.
		*/
		T* Data = nullptr;

		/*
		* The size (in bytes) of the string.
		* Excludes the null-terminating character.
		*/
		uint64 Size = 0;
	};

	/*
	* Implement a template specialization of this function in order to format any type.
	* Remember that the size SHOULD NEVER include the null-terminating character.
	* Also, there is no need for the null-terminating character to exist.
	* We are more space efficient without it.
	* 
	* @param typename T The type of characters of the returned string (char8 or char16).
	* @param typename Q The type of the class you want to format.
	* 
	* @param formatter The struct to fill with the string buffer.
	* @param value The value to be formatted.
	*/
	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q& value);
	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q* value);

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
		* It formats the variadic arguments via THArgumentString<T> struct.
		* 
		* @param type Determine the color of the message in the console.
		* @param message The message to be printed.
		* @param args The argument pack - It will be formatted.
		*/
		template<typename... Args>
		static void LogCoreMessage(Log type, const char8* message, Args&&... args)
		{
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
			GetArgumentsAsStrings(arguments, argumentsCount, std::forward<Args>(args)...);

			// Iterate over the message's characters.
			char8* formatted = (char8*)s_ArgsAllocator.GetCurrentMark();
			int64 index = -1;
			uint64 argumentIndex = 0;
			bool bInsideArg = false;
			do
			{
				index++;
				switch (message[index])
				{
					case '{':
					{
						if (bInsideArg)
						{
							// This calls the none arguments override. So it's safe.
							LogCoreMessage(Log::Warn, "Possible formatting issues!");
						}
						bInsideArg = true;
						break;
					}
					case '}':
					{
						if (!bInsideArg)
						{
							// This calls the none arguments override. So it's safe.
							LogCoreMessage(Log::Warn, "Possible formatting issues!");
						}
						bInsideArg = false;

						char8* buffer = (char8*)s_ArgsAllocator.Allocate(arguments[argumentIndex].Size);
						Memory::Copy(buffer, arguments[argumentIndex].Data, arguments[argumentIndex].Size);
						argumentIndex++;

						break;
					}
					default:
					{
						if (!bInsideArg)
						{
							char8* buffer = (char8*)s_ArgsAllocator.Allocate(sizeof(char8));
							*buffer = message[index];
						}
						break;
					}
				}
			} while (message[index] != 0);

			// Send the formatted message to the platform layer (indirectly).
			LogCoreMessage(type, formatted);
		}

	private:
		/*
		* Internal usage only.
		* 
		* @param typename T The type of the argument.
		*/
		template<typename T, typename... Args>
		static void GetArgumentsAsStrings(THFormatter<char8>* arguments, uint64& index, const T& argument, Args&&... args)
		{
			arguments[index++] << argument;
			GetArgumentsAsStrings(arguments, index, std::forward<Args>(args)...);
		}

		/*
		* Internal usage only.
		* Default case for iterating the parameter pack.
		*/
		static void GetArgumentsAsStrings(THFormatter<char8>* arguments, uint64& index) {}

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