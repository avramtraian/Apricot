#include "aepch.h"
#include "Apricot/Core/Base.h"

#if PLATFORM_WINDOWS

#include "Apricot/Core/CString.h"

namespace Apricot {

	const UTF8CHAR* FStringConversion::TCharToUtf8(const TCHAR* Str)
	{
		return nullptr;
	}

	const TCHAR* FStringConversion::Utf8ToTChar(const UTF8CHAR* Str)
	{
		return nullptr;
	}

	}

#endif