// Part of Apricot Engine. 2022-2022.
// Module: Core

#include "aepch.h"
#include "Log.h"
#include "Assert.h"

#include "Memory/ApricotMemory.h"
#include "Apricot/Containers/Strings/String.h"
#include "Platform.h"

namespace Apricot {

	// TODO: Don't do these!
	TChar ALogger::SBuffer[32000] = { 0 };
	static TChar SLogBuffer[32000] = { 0 };

	static const TChar* SLogTypes[] =
	{
		TEXT("[FATAL]: "),
		TEXT("[ERROR]: "),
		TEXT("[WARN]:  "),
		TEXT("[INFO]:  "),
		TEXT("[DEBUG]: "),
		TEXT("[TRACE]: "),
	};

	static const uint64 SLogTypeSize = 9; /* Size in characters of the log type */

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
		APlatform::ConsoleAttach();

		AE_CORE_TRACE(TEXT("Logger initialized succesfully!"));
	}

	void ALogger::Destroy()
	{
		APlatform::ConsoleFree();
	}

	void ALogger::Write(ELog Type, const TChar* Message)
	{
		uint64 MessageSize = StrLength(Message);
		AE_CORE_RASSERT(SLogTypeSize + MessageSize + 1 <= sizeof(SBuffer) / sizeof(TChar));

		MemCpy(SLogBuffer, SLogTypes[(uint8)Type], SLogTypeSize * sizeof(TChar));
		MemCpy(SLogBuffer + SLogTypeSize, Message, MessageSize * sizeof(TChar));
		SLogBuffer[SLogTypeSize + MessageSize] = '\n';

		APlatform::ConsoleWrite(SLogBuffer, SLogTypeSize + MessageSize + 1, SLogTypesColors[(uint8)Type]);
	}

	void ALogger::WriteError(ELog Type, const TChar* Message)
	{
		uint64 MessageSize = StrLength(Message);
		AE_CORE_RASSERT(SLogTypeSize + MessageSize + 1 <= sizeof(SBuffer) / sizeof(TChar));

		MemCpy(SLogBuffer, SLogTypes[(uint8)Type], SLogTypeSize * sizeof(TChar));
		MemCpy(SLogBuffer + SLogTypeSize, Message, MessageSize * sizeof(TChar));
		SLogBuffer[SLogTypeSize + MessageSize] = '\n';

		APlatform::ConsoleWriteError(SLogBuffer, SLogTypeSize + MessageSize + 1, SLogTypesColors[(uint8)Type]);
	}

}