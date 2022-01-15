// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Log.h"

#include "Memory.h"
#include "Platform.h"
#include "Engine.h"

namespace Apricot {

	char* Logger::s_MessageBuffer = nullptr;
	uint64 Logger::s_MessageBufferSize = 0;
	Memory::LinearAllocator Logger::s_ArgsAllocator;
	bool8 Logger::s_bIsConsoleEnabled;

	void Logger::Init()
	{
		s_bIsConsoleEnabled = GEngine->Config.bStartWithConsole;
		if (s_bIsConsoleEnabled)
		{
			Platform::CreateConsole();

			s_MessageBufferSize = 1024;
			s_MessageBuffer = (char8*)Memory::Allocate(s_MessageBufferSize * sizeof(char8), Memory::AllocTag::CoreSystems);

			void* argsMemoryBlock = Memory::Allocate(AE_KILOBYTES(8), Memory::AllocTag::CoreSystems);
			Memory::LinearAllocator::Create(&s_ArgsAllocator, AE_KILOBYTES(8), argsMemoryBlock);
		}
	}

	void Logger::Destroy()
	{
		if (s_bIsConsoleEnabled)
		{
			Memory::Free(s_MessageBuffer, s_MessageBufferSize * sizeof(char8), Memory::AllocTag::CoreSystems);

			Memory::Free(s_ArgsAllocator.MemoryBlock(), s_ArgsAllocator.GetTotalSize(), Memory::AllocTag::CoreSystems);
			Memory::LinearAllocator::Destroy(&s_ArgsAllocator);
		}
	}

	void Logger::LogCoreMessage(Log type, const char8* message)
	{
		if (!s_bIsConsoleEnabled)
		{
			return;
		}

		static uint32 colors[(uint8)Log::MaxEnum] = { 64, 12, 14, 2, 13, 8 };

		// Doesn't include the null-terminating character
		static const uint16 logTypeStrSize = sizeof("[FATAL]: ") - 1;
		static const char8* logTypeStrings[(uint8)Log::MaxEnum] =
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