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

		NODISCARD static void* Malloc(uint64 Size, uint64 Alignment);
		static void Free(void* MemoryBlock, uint64 Size);

		static void MemCpy(void* Destination, const void* Source, uint64 SizeBytes);
		static void MemSet(void* Destination, int32 Value, uint64 SizeBytes);
		static void MemZero(void* Destination, uint64 SizeBytes);

		static uint64 GetAllocationSize(void* Allocation);

		static void* Memory_Allocate(uint64 Size, bool8 Alligned);
		static void Memory_Free(void* Address, uint64 Size);

		static void Memory_Copy(void* Destination, const void* Source, uint64 Size);
		static void Memory_Set(void* Destination, int32 Value, uint64 Size);
		static void Memory_Zero(void* Destination, uint64 Size);

		static void Console_Attach();
		static void Console_Free();
		static void Console_Write(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color);
		static void Console_WriteError(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color);
	};

}