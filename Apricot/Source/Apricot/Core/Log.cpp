// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Log.h"
#include "Assert.h"

#include "Memory.h"
#include "Apricot/Containers/String.h"
#include "Platform.h"

namespace Apricot {
	

	// TODO: Don't do these!
	char8 ALogger::s_Buffer[32000] = { 0 };
	static char8 s_LogBuffer[32000] = {0};

	static const char8* s_LogTypes[] =
	{
		"[FATAL]: ",
		"[ERROR]: ",
		"[WARN]:  ",
		"[INFO]:  ",
		"[DEBUG]: ",
		"[TRACE]: ",
	};

	static const uint64 s_LogTypeSize = sizeof("[FATAL]: ") - 1;

	static Platform::ConsoleTextColor s_LogTypesColors[] =
	{
		Platform::ConsoleTextColor::Black_RedBg,
		Platform::ConsoleTextColor::BrightRed,
		Platform::ConsoleTextColor::PaleYellow,
		Platform::ConsoleTextColor::Green,
		Platform::ConsoleTextColor::DarkPurple,
		Platform::ConsoleTextColor::Gray
	};

	void ALogger::Init()
	{
		Platform::Console_Attach();

		AE_CORE_TRACE("Logger initialized succesfully!");
	}

	void ALogger::Destroy()
	{
		Platform::Console_Free();
	}

	void ALogger::Write(Log type, const char8* message)
	{
		uint64 messageSize = Str_Length(message);
		AE_CHECK(s_LogTypeSize + messageSize + 1 <= sizeof(s_Buffer) / sizeof(char8));

		Memory_Copy(s_LogBuffer, s_LogTypes[(uint8)type], s_LogTypeSize);
		Memory_Copy(s_LogBuffer + s_LogTypeSize, message, messageSize);
		s_LogBuffer[s_LogTypeSize + messageSize] = '\n';

		Platform::Console_Write(s_LogBuffer, s_LogTypeSize + messageSize + 1, s_LogTypesColors[(uint8)type]);
	}

	void ALogger::WriteError(Log type, const char8* message)
	{
		uint64 messageSize = Str_Length(message);
		AE_CHECK(s_LogTypeSize + messageSize + 1 <= sizeof(s_Buffer) / sizeof(char8));

		Memory_Copy(s_LogBuffer, s_LogTypes[(uint8)type], s_LogTypeSize);
		Memory_Copy(s_LogBuffer + s_LogTypeSize, message, messageSize);
		s_LogBuffer[s_LogTypeSize + messageSize] = '\n';

		Platform::Console_WriteError(s_LogBuffer, s_LogTypeSize + messageSize + 1, s_LogTypesColors[(uint8)type]);
	}

}