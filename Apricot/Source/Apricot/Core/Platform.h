#pragma once

#include "Time.h"

namespace Apricot {

	class APRICOT_API Platform
	{
	public:
		static void Init();
		static void Destroy();

		static void InitWindows(void* hInstance);

	public:
		static void MemoryCopy(void* destination, const void* source, uint64 size);
		static void MemorySet(void* destination, int32 value, uint64 size);
		static void MemoryZero(void* destination, uint64 size);

	public:
		static void* ConsoleGetNativeHandle();
		static void ConsoleOpen();
		static void ConsoleClose();
		static bool ConsoleIsOpen();

	public:
		static UTCTime TimeGetSystemUTCTime();
		static void TimeGetSystemPerformanceTime(Time& time);

	public:
		static void* WindowsGetInstance();
		static void WindowsReportError(auto ErrorCode);
	};

}