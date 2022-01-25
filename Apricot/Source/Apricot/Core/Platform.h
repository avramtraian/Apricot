// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API APlatform
	{
	public:
		enum class EConsoleTextColor : uint32
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

		static void* Memory_Allocate(uint64 Size, bool8 Alligned);
		static void Memory_Free(void* Address, uint64 Size);

		static void Memory_Copy(void* Destination, const void* Source, uint64 Size);
		static void Memory_Set(void* Destination, int32 Value, uint64 Size);
		static void Memory_Zero(void* Destination, uint64 Size);

		static void Console_Attach();
		static void Console_Free();
		static void Console_Write(const char8* Message, uint64 MessageSize, EConsoleTextColor Color);
		static void Console_WriteError(const char8* Message, uint64 MessageSize, EConsoleTextColor Color);
	};

}