#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API Platform
	{
	public:
		static void CreateConsole();
		static void PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color);
		static void PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color);
	};

}