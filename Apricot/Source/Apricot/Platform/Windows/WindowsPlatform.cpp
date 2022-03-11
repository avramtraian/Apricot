// Part of Apricot Engine. 2022-2022.
// Module: Platform

#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

#include "Apricot/Profiling/MemoryProfiler.h"

#ifdef TEXT
	#undef TEXT
#endif

#include <Windows.h>
#include <consoleapi.h>
#include <consoleapi2.h>

namespace Apricot {

	namespace Utils {

		static WORD GetConsoleColor(APlatform::EConsoleTextColor Color)
		{
			switch (Color)
			{
				case APlatform::EConsoleTextColor::Gray:        return 8;
				case APlatform::EConsoleTextColor::DarkPurple:  return 13;
				case APlatform::EConsoleTextColor::Green:       return 2;
				case APlatform::EConsoleTextColor::PaleYellow:  return 14;
				case APlatform::EConsoleTextColor::BrightRed:   return 12;
				case APlatform::EConsoleTextColor::Black_RedBg: return 64;
			}
			AE_CORE_RASSERT_NO_ENTRY();
			return 0;
		}

	}

	struct WindowsPlatformData
	{
		LARGE_INTEGER PerformanceCounterStart = { 0 };
		LARGE_INTEGER PerformanceFrequency = { 0 };

		HANDLE ConsoleOutputHandle = INVALID_HANDLE_VALUE;
		HANDLE ConsoleErrorHandle = INVALID_HANDLE_VALUE;
		bool8 bIsConsoleAttached = false;
	};
	static WindowsPlatformData SWindowsPlatformData;

	void APlatform::Init()
	{
		AE_CORE_VERIFY(QueryPerformanceCounter(&SWindowsPlatformData.PerformanceCounterStart));

		AE_CORE_VERIFY(QueryPerformanceFrequency(&SWindowsPlatformData.PerformanceFrequency));
	}

	void APlatform::Destroy()
	{
		
	}

	void* APlatform::Malloc(uint64 Size, uint64 Alignment)
	{
		if (Size == 0)
		{
			return nullptr;
		}
		AMemoryProfiler::SubmitHeapAllocation(Size, Alignment);
		return ::operator new(Size);
	}

	void APlatform::Free(void* MemoryBlock, uint64 Size)
	{
		AMemoryProfiler::SubmitHeapDeallocation(MemoryBlock, Size);
		::operator delete(MemoryBlock, Size);
	}

	void APlatform::MemCpy(void* Destination, const void* Source, uint64 SizeBytes)
	{
		memcpy(Destination, Source, SizeBytes);
	}

	void APlatform::MemSet(void* Destination, int32 Value, uint64 SizeBytes)
	{
		memset(Destination, Value, SizeBytes);
	}

	void APlatform::MemZero(void* Destination, uint64 SizeBytes)
	{
		memset(Destination, 0, SizeBytes);
	}

	uint64 APlatform::GetAllocationSize(void* Allocation)
	{
		return _msize(Allocation);
	}

	NODISCARD Time APlatform::GetSystemPerformanceTime()
	{
		LARGE_INTEGER performanceTimerNow;
		AE_CORE_VERIFY(QueryPerformanceCounter(&performanceTimerNow));

		LARGE_INTEGER elapsedNanoseconds;
		// Get the number of ticks since the initialization of the platform
		elapsedNanoseconds.QuadPart = performanceTimerNow.QuadPart - SWindowsPlatformData.PerformanceCounterStart.QuadPart;

		// Divide the number of ticks by the frequency, resulting in seconds. Multiplying the ticks before makes the result be in nanoseconds. Accuracy
		elapsedNanoseconds.QuadPart = (elapsedNanoseconds.QuadPart * 1000000000) / SWindowsPlatformData.PerformanceFrequency.QuadPart;

		return elapsedNanoseconds.QuadPart;
	}

	void APlatform::SleepFor(Time duration)
	{
		Sleep((DWORD)(duration.Miliseconds()));
	}

	bool APlatform::IsConsoleAvailable()
	{
		return SWindowsPlatformData.bIsConsoleAttached;
	}

	void APlatform::ConsoleAttach()
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		SWindowsPlatformData.ConsoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SWindowsPlatformData.ConsoleErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
		SWindowsPlatformData.bIsConsoleAttached = true;
	}


	void APlatform::ConsoleFree()
	{
		if (!SWindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		FreeConsole();

		SWindowsPlatformData.ConsoleOutputHandle = INVALID_HANDLE_VALUE;
		SWindowsPlatformData.ConsoleErrorHandle = INVALID_HANDLE_VALUE;
		SWindowsPlatformData.bIsConsoleAttached = false;
	}

	void APlatform::ConsoleWrite(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color)
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(SWindowsPlatformData.ConsoleOutputHandle, Utils::GetConsoleColor(Color));

			DWORD NumberOfCharsWritten = 0;
			WriteConsole(SWindowsPlatformData.ConsoleOutputHandle, Message, (DWORD)MessageSize, &NumberOfCharsWritten, NULL);
		}
	}

	void APlatform::ConsoleWriteError(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color)
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(SWindowsPlatformData.ConsoleErrorHandle, Utils::GetConsoleColor(Color));

			DWORD NumberOfCharsWritten = 0;
			WriteConsole(SWindowsPlatformData.ConsoleErrorHandle, Message, (DWORD)MessageSize, &NumberOfCharsWritten, NULL);
		}
	}

}

#endif