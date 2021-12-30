#pragma once

#include "Base.h"
#include "Time.h"

namespace Apricot {

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
	};

}