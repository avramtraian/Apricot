// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

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
			AE_CHECK_NO_ENTRY();
			return 0;
		}

	}

	struct WindowsPlatformData
	{
		HANDLE Console_OutputHandle = INVALID_HANDLE_VALUE;
		HANDLE Console_ErrorHandle = INVALID_HANDLE_VALUE;
		bool8 bIsConsoleAttached = false;
	};
	static WindowsPlatformData SWindowsPlatformData;

	void APlatform::Init()
	{
		
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
		return ::operator new(Size);
	}

	void APlatform::Free(void* MemoryBlock, uint64 Size)
	{
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

	void APlatform::Console_Attach()
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		SWindowsPlatformData.Console_OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SWindowsPlatformData.Console_ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
		SWindowsPlatformData.bIsConsoleAttached = true;
	}


	void APlatform::Console_Free()
	{
		if (!SWindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		FreeConsole();

		SWindowsPlatformData.Console_OutputHandle = INVALID_HANDLE_VALUE;
		SWindowsPlatformData.Console_ErrorHandle = INVALID_HANDLE_VALUE;
		SWindowsPlatformData.bIsConsoleAttached = false;
	}

	void APlatform::Console_Write(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color)
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(SWindowsPlatformData.Console_OutputHandle, Utils::GetConsoleColor(Color));

			DWORD NumberOfCharsWritten = 0;
			WriteConsole(SWindowsPlatformData.Console_OutputHandle, Message, (DWORD)MessageSize, &NumberOfCharsWritten, NULL);
		}
	}

	void APlatform::Console_WriteError(const TChar* Message, uint64 MessageSize, EConsoleTextColor Color)
	{
		if (SWindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(SWindowsPlatformData.Console_ErrorHandle, Utils::GetConsoleColor(Color));

			DWORD NumberOfCharsWritten = 0;
			WriteConsole(SWindowsPlatformData.Console_ErrorHandle, Message, (DWORD)MessageSize, &NumberOfCharsWritten, NULL);
		}
	}

	void* APlatform::Memory_Allocate(uint64 Size, bool8 Alligned)
	{
		if (Size == 0)
		{
			return nullptr;
		}
		return ::operator new(Size);
	}

	void APlatform::Memory_Free(void* Address, uint64 Size)
	{
		::operator delete(Address, Size);
	}

	void APlatform::Memory_Copy(void* Destination, const void* Source, uint64 Size)
	{
		memcpy(Destination, Source, Size);
	}

	void APlatform::Memory_Set(void* Destination, int32 Value, uint64 Size)
	{
		memset(Destination, Value, Size);
	}

	void APlatform::Memory_Zero(void* Destination, uint64 Size)
	{
		ZeroMemory(Destination, Size);
	}

}

#endif