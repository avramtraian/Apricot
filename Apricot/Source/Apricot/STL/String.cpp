#include "aepch.h"
#include "String.h"

// TODO: Remove.
#include <string.h>

namespace Apricot {

	APRICOT_API uint64 Str_Length(const char8* string)
	{
		return strlen(string);
	}

	APRICOT_API uint64 Str_Length(const char16* string)
	{
		return wcslen(string);
	}
	
}