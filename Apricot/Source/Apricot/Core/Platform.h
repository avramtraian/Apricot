#pragma once

#include "Time.h"

namespace Apricot {

	class APRICOT_API Platform
	{
	public:
		static void Init();
		static void Destroy();

	public:
		static void MemoryCopy(void* destination, const void* source, uint64 size);
		static void MemorySet(void* destination, int32 value, uint64 size);
		static void MemoryZero(void* destination, uint64 size);

	public:
		static void* ConsoleGetNativeHandle();

	public:
		static UTCTime TimeGetSystemUTCTime();
		static void TimeGetSystemPerformanceTime(Time& time);
	};

}