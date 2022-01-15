// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Assert.h"

#include "Engine.h"
#include "Platform.h"

// Note: It should be already included by the pch.
#ifdef AE_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Apricot {

#ifdef AE_PLATFORM_WINDOWS

	APRICOT_API void OnCoreAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message)
	{
		Memory::LinearAllocator allocator(AE_KILOBYTES(32));
		const char8* formatted = RawStringFormatter::Format(allocator, "Assertion failed! Apricot Engine has crashed!\n\n{}\n\n{}\n\n[FILE] {}\n[LINE] {}\n[FUNC] {}\n", condition, message, file, line, funcSig);

		auto button = MessageBoxA(NULL, formatted, "Apricot Engine crashed!", MB_ICONERROR | MB_OK);
		if (button == IDOK)
		{
			GEngine->OnForceShutdown();
		}
	}

	APRICOT_API void OnBaseAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message)
	{
		auto button = MessageBoxA(NULL, message, "Apricot Engine crashed!", MB_ICONERROR | MB_OK);
		if (button == IDOK)
		{
			GEngine->OnForceShutdown();
		}
	}

#endif

}