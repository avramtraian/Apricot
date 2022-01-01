// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Assert.h"

#include "Engine.h"
#include "Platform.h"

namespace Apricot {

	void Assert::OnFatalAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message)
	{
		Memory::LinearAllocator allocator(AE_KILOBYTES(32));
		THFormatter<char8>* formatters = (THFormatter<char8>*)allocator.Allocate(5 * sizeof(THFormatter<char8>));
		for (uint64 index = 0; index < 5; index++)
		{
			Memory::PlacementNew<THFormatter<char8>>(formatters + index, &allocator);
		}

		uint64 argsCount = 0;
		const char8* formatted = RawStringFormatter::Format(allocator, formatters, 5, argsCount, "Assertion failed! Apricot Engine has crashed!\n\n{}\n\n{}\n\n[FILE] {}\n[LINE] {}\n[FUNC] {}\n", condition, message, file, line, funcSig);

		if (Platform::DisplayMessageBox("Apricot Engine crashed! Assertion failed!", formatted, MessageBoxFlags::Error) == MessageBoxButton::Ok)
		{
			GEngine->OnForceShutdown();
		}
	}

}