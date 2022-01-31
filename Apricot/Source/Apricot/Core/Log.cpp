// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Log.h"
#include "Assert.h"

#include "Memory/Memory.h"
#include "Apricot/Containers/String.h"
#include "Platform.h"

namespace Apricot {
	

	// TODO: Don't do these!
	char8 ALogger::SBuffer[32000] = { 0 };
	static char8 SLogBuffer[32000] = {0};

	static const char8* SLogTypes[] =
	{
		"[FATAL]: ",
		"[ERROR]: ",
		"[WARN]:  ",
		"[INFO]:  ",
		"[DEBUG]: ",
		"[TRACE]: ",
	};

	static const uint64 SLogTypeSize = sizeof("[FATAL]: ") - 1;

	static APlatform::EConsoleTextColor SLogTypesColors[] =
	{
		APlatform::EConsoleTextColor::Black_RedBg,
		APlatform::EConsoleTextColor::BrightRed,
		APlatform::EConsoleTextColor::PaleYellow,
		APlatform::EConsoleTextColor::Green,
		APlatform::EConsoleTextColor::DarkPurple,
		APlatform::EConsoleTextColor::Gray
	};

	void ALogger::Init()
	{
		APlatform::Console_Attach();

		AE_CORE_TRACE("Logger initialized succesfully!");
	}

	void ALogger::Destroy()
	{
		APlatform::Console_Free();
	}

	void ALogger::Write(ELog Type, const char8* Message)
	{
		uint64 MessageSize = Str_Length(Message);
		AE_CHECK(SLogTypeSize + MessageSize + 1 <= sizeof(SBuffer) / sizeof(char8));

		Memory_Copy(SLogBuffer, SLogTypes[(uint8)Type], SLogTypeSize);
		Memory_Copy(SLogBuffer + SLogTypeSize, Message, MessageSize);
		SLogBuffer[SLogTypeSize + MessageSize] = '\n';

		APlatform::Console_Write(SLogBuffer, SLogTypeSize + MessageSize + 1, SLogTypesColors[(uint8)Type]);
	}

	void ALogger::WriteError(ELog Type, const char8* Message)
	{
		uint64 MessageSize = Str_Length(Message);
		AE_CHECK(SLogTypeSize + MessageSize + 1 <= sizeof(SBuffer) / sizeof(char8));

		Memory_Copy(SLogBuffer, SLogTypes[(uint8)Type], SLogTypeSize);
		Memory_Copy(SLogBuffer + SLogTypeSize, Message, MessageSize);
		SLogBuffer[SLogTypeSize + MessageSize] = '\n';

		APlatform::Console_WriteError(SLogBuffer, SLogTypeSize + MessageSize + 1, SLogTypesColors[(uint8)Type]);
	}

}