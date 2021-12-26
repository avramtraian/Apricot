#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Platform.h"

#include <Windows.h>
#include <stdio.h>

namespace Apricot {

	void Platform::CreateConsole()
	{
		AttachConsole(ATTACH_PARENT_PROCESS);
		AllocConsole();

		FILE* o;
		freopen_s(&o, "CON", "w", stdout);
		FILE* e;
		freopen_s(&e, "CON", "w", stderr);
		FILE* i;
		freopen_s(&i, "CON", "r", stdin);
	}

	void Platform::PrintToConsole(const char* buffer, uint64 bufferSize, uint32 color)
	{
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleA(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(char)), NULL, NULL);
	}

	void Platform::PrintToConsole(const wchar_t* buffer, uint64 bufferSize, uint32 color)
	{
		static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(consoleHandle, (WORD)color);
		WriteConsoleW(consoleHandle, buffer, (DWORD)(bufferSize / sizeof(wchar_t)), NULL, NULL);
	}

}

#endif