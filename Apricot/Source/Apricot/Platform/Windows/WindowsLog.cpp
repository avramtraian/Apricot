#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Log.h"

#include <Windows.h>

namespace Apricot {

	void OutputSink::SyncWriteToConsole(IOStream& stream)
	{
		HANDLE consoleHandle = (HANDLE)stream.Handle;
		if (consoleHandle == INVALID_HANDLE_VALUE) return;

		auto& color = m_Specification.ColorScheme.Colors[(uint32)stream.Severity];
		SetConsoleTextAttribute(consoleHandle, (WORD)color.Background * 16u + (WORD)color.Foreground);

		switch (m_Specification.TextEncoding)
		{
			case Encoding::ASCII:
			{
				DWORD writtenChars = 0;
				WriteConsoleA(consoleHandle, (uint8*)stream.m_SinkBuffer->Data(), (DWORD)stream.m_SinkBuffer->Offset, &writtenChars, NULL);
				break;
			}
			case Encoding::Unicode:
			{
				DWORD writtenChars = 0;
				WriteConsoleW(consoleHandle, (uint8*)stream.m_SinkBuffer->Data(), (DWORD)stream.m_SinkBuffer->Offset, &writtenChars, NULL);
				break;
			}
		}
	}

}

#endif