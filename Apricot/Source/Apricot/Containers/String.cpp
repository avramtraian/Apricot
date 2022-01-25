// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "String.h"

// TODO: Remove.
#include <string.h>

namespace Apricot {

	APRICOT_API uint64 Str_Length(const char8* String)
	{
		return strlen(String);
	}

	APRICOT_API uint64 Str_Length(const char16* String)
	{
		return wcslen(String);
	}
	
}