// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

#include <Windows.h>
#include <stdio.h>

namespace Apricot {

	struct HPlatformData
	{
		LARGE_INTEGER PerformanceCounterStart;
		LARGE_INTEGER PerformanceFrequency;
	};
	static HPlatformData* s_Platform = nullptr;

	void Platform::Init()
	{
		s_Platform = (HPlatformData*)Memory::Allocate(sizeof(HPlatformData), Memory::AllocTag::CoreSystems);

		BOOL result = QueryPerformanceCounter(&s_Platform->PerformanceCounterStart);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceCounter. Aborting.");

		result = QueryPerformanceFrequency(&s_Platform->PerformanceFrequency);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceFrequency. Aborting.");
	}

	void Platform::Destroy()
	{
		Memory::Free(s_Platform, sizeof(HPlatformData), Memory::AllocTag::CoreSystems);
	}

	void Platform::CreateConsole()
	{
		AttachConsole(ATTACH_PARENT_PROCESS);
		AllocConsole();

		FILE* o;
		freopen_s(&o, "CON", "w", stdout);
		FILE* e;
		freopen_s(&e, "CON", "w", stderr);
		FILE* i;
		freopen_s(&i, "CON", "r", stdin);
	}

	void Platform::PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color)
	{
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleA(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(char)), NULL, NULL);
	}

	void Platform::PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color)
	{
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleW(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(wchar_t)), NULL, NULL);
	}

	HTime Platform::GetPerformanceTime()
	{
		LARGE_INTEGER performanceTimerNow;
		BOOL result = QueryPerformanceCounter(&performanceTimerNow);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceFrequency. Aborting.");

		LARGE_INTEGER elapsedNanoseconds;
		// Get the number of ticks
		elapsedNanoseconds.QuadPart = performanceTimerNow.QuadPart - s_Platform->PerformanceCounterStart.QuadPart;

		// Transforms the "time" in nanoseconds (from seconds).
		elapsedNanoseconds.QuadPart *= 1000000000;

		// Divide the number of ticks by the frequency. Without the above multiplication, it will return
		// an approximation in seconds.
		elapsedNanoseconds.QuadPart /= s_Platform->PerformanceFrequency.QuadPart;

		return elapsedNanoseconds.QuadPart;
	}

	void Platform::SleepFor(HTime duration)
	{
		Sleep((DWORD)duration.Miliseconds());
	}

	namespace Utils {

		static uint32 ConvertMessageBoxFlags(MessageBoxFlags flags)
		{
			switch (flags)
			{
				case MessageBoxFlags::None:  return 0;
				case MessageBoxFlags::Error: return MB_ICONERROR;
			}
			AE_CORE_ASSERT_RETURN(false, 0, "Invalid MessageBoxFlags!");
		}

	}

	MessageBoxButton Platform::DisplayMessageBox(const char8* title, const char8* message, MessageBoxFlags flags)
	{
		switch (MessageBoxA(NULL, message, title, Utils::ConvertMessageBoxFlags(flags)))
		{
			case IDABORT:    return MessageBoxButton::Abort;
			case IDCANCEL:   return MessageBoxButton::Cancel;
			case IDCONTINUE: return MessageBoxButton::Continue;
			case IDIGNORE:   return MessageBoxButton::Ignore;
			case IDYES:      return MessageBoxButton::Yes;
			case IDNO:       return MessageBoxButton::No;
			case IDOK:       return MessageBoxButton::Ok;
			case IDRETRY:    return MessageBoxButton::Retry;
			case IDTRYAGAIN: return MessageBoxButton::TryAgain;
			default:         return MessageBoxButton::None;
		};
	}

}

#endif