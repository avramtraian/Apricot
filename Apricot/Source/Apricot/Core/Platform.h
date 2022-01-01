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
		static void Init();
		static void Destroy();

		static void CreateConsole();
		static void PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color);
		static void PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color);

		static HTime GetPerformanceTime();

		static void SleepFor(HTime duration);

		static MessageBoxButton DisplayMessageBox(const char8* title, const char8* message, MessageBoxFlags flags);
	};

}