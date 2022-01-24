// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

#include <Windows.h>

namespace Apricot {

	struct WindowsPlatformData
	{
		HANDLE Console_OutputHandle = INVALID_HANDLE_VALUE;
		HANDLE Console_ErrorHandle = INVALID_HANDLE_VALUE;
		bool8 bIsConsoleAttached = false;
	};
	static WindowsPlatformData s_WindowsPlatformData;

	void Platform::Init()
	{
		
	}

	void Platform::Destroy()
	{
		
	}

	void* Platform::Memory_Allocate(uint64 size, bool8 alligned)
	{
		if (size == 0)
		{
			return nullptr;
		}
		return ::operator new(size);
	}

	void Platform::Memory_Free(void* address, uint64 size)
	{
		::operator delete(address, size);
	}

	void Platform::Memory_Copy(void* destination, const void* source, uint64 size)
	{
		memcpy(destination, source, size);
	}

	void Platform::Memory_Set(void* destination, int32 value, uint64 size)
	{
		memset(destination, value, size);
	}

	void Platform::Memory_Zero(void* destination, uint64 size)
	{
		ZeroMemory(destination, size);
	}

	void Platform::Console_Attach()
	{
		if (s_WindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		s_WindowsPlatformData.Console_OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		s_WindowsPlatformData.Console_ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
		s_WindowsPlatformData.bIsConsoleAttached = true;
	}


	void Platform::Console_Free()
	{
		if (!s_WindowsPlatformData.bIsConsoleAttached)
		{
			return;
		}

		FreeConsole();

		s_WindowsPlatformData.Console_OutputHandle = INVALID_HANDLE_VALUE;
		s_WindowsPlatformData.Console_ErrorHandle = INVALID_HANDLE_VALUE;
		s_WindowsPlatformData.bIsConsoleAttached = false;
	}

	void Platform::Console_Write(const char8* message, uint64 messageSize, ConsoleTextColor color)
	{
		if (s_WindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(s_WindowsPlatformData.Console_OutputHandle, (WORD)color);

			DWORD numberOfCharsWritten = 0;
			WriteConsoleA(s_WindowsPlatformData.Console_OutputHandle, message, (DWORD)messageSize, &numberOfCharsWritten, NULL);
		}
	}

	void Platform::Console_WriteError(const char8* message, uint64 messageSize, ConsoleTextColor color)
	{
		if (s_WindowsPlatformData.bIsConsoleAttached)
		{
			SetConsoleTextAttribute(s_WindowsPlatformData.Console_ErrorHandle, (WORD)color);

			DWORD numberOfCharsWritten = 0;
			WriteConsoleA(s_WindowsPlatformData.Console_ErrorHandle, message, (DWORD)messageSize, &numberOfCharsWritten, NULL);
		}
	}

}

#endif