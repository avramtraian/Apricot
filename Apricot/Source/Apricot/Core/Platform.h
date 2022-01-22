#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API Platform
	{
	public:
		enum class ConsoleTextColor : uint32
		{
			None = 0,

			Gray        = 8,
			DarkPurple  = 13,
			Green       = 2,
			PaleYellow  = 14,
			BrightRed   = 12,
			Black_RedBg = 64
		};

	public:
		static void Init();
		static void Destroy();

		static void* Memory_Allocate(uint64 size, bool8 alligned);
		static void Memory_Free(void* address, uint64 size);

		static void Memory_Copy(void* destination, const void* source, uint64 size);
		static void Memory_Set(void* destination, int32 value, uint64 size);
		static void Memory_Zero(void* destination, uint64 size);

		static void Console_Attach();
		static void Console_Free();
		static void Console_Write(const char8* message, uint64 messageSize, ConsoleTextColor color);
		static void Console_WriteError(const char8* message, uint64 messageSize, ConsoleTextColor color);
	};

}