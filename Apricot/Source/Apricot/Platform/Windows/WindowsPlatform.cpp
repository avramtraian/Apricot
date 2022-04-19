#include "aepch.h"

#include "Apricot/Core/Platform.h"

#include "WindowsHeaders.h"

#include <string.h>

namespace Apricot {

	struct WindowsPlatformData
	{
		LARGE_INTEGER PerformanceCounterStart = { 0 };
		double PerformanceFrequency = 0.0;

		HANDLE ConsoleHandle = INVALID_HANDLE_VALUE;
		HANDLE ErrorConsoleHandle = INVALID_HANDLE_VALUE;
		bool IsConsoleOpen = false;

		HANDLE Instance = INVALID_HANDLE_VALUE;
	};
	static WindowsPlatformData s_WindowsData;

	void Platform::Init()
	{
		LARGE_INTEGER frequency = { 0 };
		WIN_CHECK_RESULT(QueryPerformanceCounter(&s_WindowsData.PerformanceCounterStart));
		WIN_CHECK_RESULT(QueryPerformanceFrequency(&frequency));
		s_WindowsData.PerformanceFrequency = (1.0 / (double)frequency.QuadPart) * (double)Sec2Nano;
	}

	void Platform::Destroy()
	{
		if (s_WindowsData.ConsoleHandle != INVALID_HANDLE_VALUE && s_WindowsData.IsConsoleOpen)
		{
			// Default Console Color
			WIN_CHECK_RESULT(SetConsoleTextAttribute(s_WindowsData.ConsoleHandle, 7));

#ifdef AE_ALLOW_CONSOLE
			ConsoleClose();
#endif
		}
	}

	void Platform::InitWindows(void* hInstance)
	{
		Init();
		s_WindowsData.Instance = hInstance;

#ifdef AE_ALLOW_CONSOLE
		ConsoleOpen();
#endif
	}

	void Platform::MemoryCopy(void* destination, const void* source, uint64 size)
	{
		memcpy(destination, source, size);
	}

	void Platform::MemorySet(void* destination, int32 value, uint64 size)
	{
		memset(destination, value, size);
	}

	void Platform::MemoryZero(void* destination, uint64 size)
	{
		memset(destination, 0, size);
	}

	void* Platform::ConsoleGetNativeHandle()
	{
		return (void*)s_WindowsData.ConsoleHandle;
	}

	void Platform::ConsoleOpen()
	{
		if (!s_WindowsData.IsConsoleOpen)
		{
			WIN_CHECK_RESULT(AllocConsole());
			s_WindowsData.ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			s_WindowsData.ErrorConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
			s_WindowsData.IsConsoleOpen = true;
		}
	}

	void Platform::ConsoleClose()
	{
		if (s_WindowsData.IsConsoleOpen)
		{
			WIN_CHECK_RESULT(FreeConsole());
			s_WindowsData.IsConsoleOpen = false;
		}
	}

	bool Platform::ConsoleIsOpen()
	{
		return s_WindowsData.IsConsoleOpen;
	}

	UTCTime Platform::TimeGetSystemUTCTime()
	{
		UTCTime time;
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		time.Year   = sysTime.wYear;
		time.Month  = sysTime.wMonth;
		time.Day    = sysTime.wDay;
		time.Hour   = sysTime.wHour;
		time.Minute = sysTime.wMinute;
		time.Second = sysTime.wSecond;

		return time;
	}

	void Platform::TimeGetSystemPerformanceTime(Time& time)
	{
		LARGE_INTEGER performanceTimerNow;
		WIN_CHECK_RESULT(QueryPerformanceCounter(&performanceTimerNow));

		uint64 elapsedNanoseconds = { 0 };

		elapsedNanoseconds = performanceTimerNow.QuadPart - s_WindowsData.PerformanceCounterStart.QuadPart;

		// PerformanceFrequency represents the ticks per SECOND. We want the result in nanoseconds (for accuracy), so we multiply with Sec2Nano first
		elapsedNanoseconds = (uint64)(elapsedNanoseconds * s_WindowsData.PerformanceFrequency);

		time.Nanoseconds = elapsedNanoseconds;
	}

	void* Platform::WindowsGetInstance()
	{
		return s_WindowsData.Instance;
	}

	void Platform::WindowsReportError(auto ErrorCode)
	{
		LPSTR MessageBuffer = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&MessageBuffer, 0, NULL
		);
		LocalFree(MessageBuffer);
	}

}