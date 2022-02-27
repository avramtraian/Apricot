// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"
#include "Timestep.h"

namespace Apricot {
	
	/**
	* C++ Core Engine Architecture
	* 
	* Engine Platform
	*/
	class APRICOT_API APlatform
	{
	/* Typedefs */
	public:
		enum class EConsoleTextColor : uint8
		{
			None = 0,

			Gray,
			DarkPurple,
			Green,
			PaleYellow,
			BrightRed,
			Black_RedBg,

			MaxEnum
		};

	/* Init & Destroy */
	public:
		static void Init();
		static void Destroy();

	/* Memory accesses */
	public:
		NODISCARD static void* Malloc(uint64 Size, uint64 Alignment);
		static void Free(void* MemoryBlock, uint64 Size);

		static void MemCpy(void* Destination, const void* Source, uint64 SizeBytes);
		static void MemSet(void* Destination, int32 Value, uint64 SizeBytes);
		static void MemZero(void* Destination, uint64 SizeBytes);

		static uint64 GetAllocationSize(void* Allocation);

	/* Timing */
	public:
		static NODISCARD Time GetSystemPerformanceTime();

		static void SleepFor(Time duration);

	/* Console */
	public:
		static void ConsoleAttach();
		static void ConsoleFree();
		static void ConsoleWrite(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color);
		static void ConsoleWriteError(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color);
	};

}