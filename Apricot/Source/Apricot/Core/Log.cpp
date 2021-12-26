#include "aepch.h"
#include "Log.h"

#include "Memory.h"
#include "Platform.h"

namespace Apricot {

	char* Logger::s_MessageBuffer = nullptr;
	uint64 Logger::s_MessageBufferSize = 0;

	void Logger::Init()
	{
		Platform::CreateConsole();

		s_MessageBufferSize = 1024;
		s_MessageBuffer = (char*)Memory::Allocate(s_MessageBufferSize, Memory::AllocTag::CoreSystems);
	}

	void Logger::Destroy()
	{
		Memory::Free(s_MessageBuffer, s_MessageBufferSize, Memory::AllocTag::CoreSystems);
	}

	void Logger::LogCoreMessage(Log type, const char* message)
	{
		static uint32 colors[(uint8)Log::MaxEnum] = { 64, 14, 12, 2, 13, 8 };

		// Doesn't include the null-terminating character
		static const uint16 logTypeStrSize = sizeof("[FATAL]: ") - 1;
		static const char* logTypeStrings[(uint8)Log::MaxEnum] =
		{
			"[FATAL]: ",
			"[ERROR]: ",
			"[WARN]:  ",
			"[INFO]:  ",
			"[DEBUG]: ",
			"[TRACE]: ",
		};

		uint64 messageSize = strlen(message);
		AE_CORE_VERIFY(logTypeStrSize + messageSize < s_MessageBufferSize - 1, "Message buffer overflow!");

		Memory::Copy(s_MessageBuffer, logTypeStrings[(uint8)type], logTypeStrSize);
		Memory::Copy(s_MessageBuffer + logTypeStrSize, message, messageSize);
		s_MessageBuffer[logTypeStrSize + messageSize] = '\n';

		Platform::PrintToConsole(s_MessageBuffer, logTypeStrSize + messageSize + 1, colors[(uint8)type]);
	}

}