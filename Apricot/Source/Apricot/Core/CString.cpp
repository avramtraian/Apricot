#include "aepch.h"
#include "CString.h"

namespace Apricot {

	template<>
	SizeType FCStringCalls::Strlen(const TCHAR* String)
	{
		if (String == nullptr)
		{
			return 0;
		}
		return wcslen(String);
	}

	template<>
	SizeType FCStringAnsiCalls::Strlen(const ANSICHAR* String)
	{
		if (String == nullptr)
		{
			return 0;
		}
		return strlen(String);
	}

	template<>
	bool FCStringCalls::Strcmp(const TCHAR* StringA, const TCHAR* StringB)
	{
		return wcscmp(StringA, StringB);
	}

	template<>
	bool FCStringAnsiCalls::Strcmp(const ANSICHAR* StringA, const ANSICHAR* StringB)
	{
		return strcmp(StringA, StringB);
	}

	const ANSICHAR* FStringConversion::TCharToAnsi(const TCHAR* Str)
	{
	
		return nullptr;
	}

	const TCHAR* FStringConversion::AnsiToTChar(const ANSICHAR* Str)
	{
		return nullptr;
	}

}