// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

///////////////////////////////////////////////////////////////////////////////
//////////////----  APRICOT PLATFORM WINDOWS IMPLEMENTATION  ----//////////////
///////////////----  THIS FILE IS COMPILED ONLY ON WINDOWS  ----///////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

#include "Apricot/Filesystem/Filesystem.h"

#include <Windows.h>
#include <stdio.h>

namespace Apricot {

	struct HPlatformData
	{
		LARGE_INTEGER PerformanceCounterStart;
		LARGE_INTEGER PerformanceFrequency;
	};
	static HPlatformData s_Platform;

	void Platform::Init()
	{
		BOOL result = QueryPerformanceCounter(&s_Platform.PerformanceCounterStart);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceCounter. Aborting.");

		result = QueryPerformanceFrequency(&s_Platform.PerformanceFrequency);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceFrequency. Aborting.");
	}

	void Platform::Destroy()
	{
		
	}

	void Platform::CreateConsole()
	{
#ifdef AE_ENABLE_CONSOLE
		AttachConsole(ATTACH_PARENT_PROCESS);
		AllocConsole();

		FILE* o;
		freopen_s(&o, "CON", "w", stdout);
		FILE* e;
		freopen_s(&e, "CON", "w", stderr);
		FILE* i;
		freopen_s(&i, "CON", "r", stdin);
#endif
	}

	void Platform::PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color)
	{
#ifdef AE_ENABLE_CONSOLE
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleA(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(char)), NULL, NULL);
#endif
	}

	void Platform::PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color)
	{
#ifdef AE_ENABLE_CONSOLE
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleW(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(wchar_t)), NULL, NULL);
#endif
	}

	HTime Platform::GetPerformanceTime()
	{
		LARGE_INTEGER performanceTimerNow;
		BOOL result = QueryPerformanceCounter(&performanceTimerNow);
		AE_CORE_VERIFY(result, "Couldn't QueryPerformanceFrequency. Aborting.");

		LARGE_INTEGER elapsedNanoseconds;
		// Get the number of ticks
		elapsedNanoseconds.QuadPart = performanceTimerNow.QuadPart - s_Platform.PerformanceCounterStart.QuadPart;

		// Transforms the "time" in nanoseconds (from seconds).
		elapsedNanoseconds.QuadPart *= 1000000000;

		// Divide the number of ticks by the frequency. Without the above multiplication, it will return
		// an approximation in seconds.
		elapsedNanoseconds.QuadPart /= s_Platform.PerformanceFrequency.QuadPart;

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
			return 0;
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

	bool8 Platform::ReadConfigFile(const char16* filepath, ConfigFileKey* keys, uint64 keysCount)
	{
		for (uint64 index = 0; index < keysCount; index++)
		{
			ConfigFileKey& key = keys[index];
			BOOL result = GetPrivateProfileString(key.Section, key.Key, key.Default, key.Value, (DWORD)key.ValueSize, filepath);

			if (!result)
			{
				switch (GetLastError())
				{
					case ERROR_FILE_NOT_FOUND:
					{
						AE_CORE_ERROR("Failed to read configuration! Section: '{}', Key: '{}'. ERROR_FILE_NOT_FOUND");
						return false;
					}
					default:
					{
						AE_CORE_ERROR("Failed to read configuration! Section: '{}', Key: '{}'. ERROR_UNKNOWN.");
						return false;
					}
				}
			}
		}

		return true;
	}

	bool8 Platform::WriteConfigFile(const char16* filepath, ConfigFileKey* keys, uint64 keysCount)
	{
		if (!Filesystem::Exists(filepath))
		{
			Filesystem::CreateDirectories(Filesystem::Path(filepath).Parent());
		}

		for (uint64 index = 0; index < keysCount; index++)
		{
			ConfigFileKey& key = keys[index];
			BOOL result = WritePrivateProfileString(key.Section, key.Key, key.Value, filepath);

			if (!result)
			{
				AE_CORE_ERROR("Failed to read configuration! Section: '{}', Key: '{}'. ERROR_UNKNOWN.");
				return false;
			}
		}

		return true;
	}

}

#endif