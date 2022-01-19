// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Time.h"

namespace Apricot {

	enum class MessageBoxButton : uint8
	{
		None = 0,
		Abort, Cancel, Continue, Ignore,
		Yes, No, Ok,
		Retry, TryAgain
	};

	enum class MessageBoxFlags : uint8
	{
		None = 0,
		Error,
	};

	class APRICOT_API Platform
	{
	public:
		struct ConfigFileKey
		{
			ConfigFileKey() {}

			ConfigFileKey(const char16* section, const char16* key, const char16* def, const char16* value, uint64 valueSize)
				: Section(section), Key(key), Default(def), Value((char16*)value), ValueSize(valueSize) {}

			/*
			* The section where to read/write the configuration.
			*/
			const char16* Section = nullptr;

			/*
			* The key of the configuration.
			*/
			const char16* Key = nullptr;

			/*
			* The default value for the configuration.
			*/
			const char16* Default = nullptr;

			/*
			* The value buffer for the configuration.
			*/
			char16* Value = nullptr;
			/*
			* The size (in elements) of the value buffer.
			*/
			uint64 ValueSize = 0;
		};

	public:
		static void Init();
		static void Destroy();

		static void CreateConsole();
		static void PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color);
		static void PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color);

		static HTime GetPerformanceTime();

		static void SleepFor(HTime duration);

		static MessageBoxButton DisplayMessageBox(const char8* title, const char8* message, MessageBoxFlags flags);

		/*
		* Reads the configurations from a file.
		* 
		* @param filepath 
		* @param keys 
		* @param keysCount 
		* 
		* @return The success of the function. True = success, False = error.
		*/
		static bool8 ReadConfigFile(const char16* filepath, ConfigFileKey* keys, uint64 keysCount);

		/*
		* Writes the configurations to a file.
		* 
		* @param filepath 
		* @param keys 
		* @param keysCount 
		* 
		* @return The success of the function. True = success, False = error.
		*/
		static bool8 WriteConfigFile(const char16* filepath, ConfigFileKey* keys, uint64 keysCount);
	};

}