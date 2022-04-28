#include "aepch.h"
#include "CString.h"

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

const ANSICHAR* FStringConversion::TCharToAnsi(const TCHAR* Str)
{
	
	return nullptr;
}

const TCHAR* FStringConversion::AnsiToTChar(const ANSICHAR* Str)
{
	return nullptr;
}