#pragma once

#include "Core.h"

#define TCHAR_TO_ANSI(TCHARStr) (FStringConversion::TCharToAnsi((const TCHAR*)TCHARStr))
#define ANSI_TO_TCHAR(ANSIStr)  (FStringConversion::AnsiToTChar((const ANSICHAR*)ANSIStr))
#define TCHAR_TO_UTF8(TCHARStr) (FStringConversion::TCharToUtf8((const TCHAR*)TCHARStr))
#define UTF8_TO_TCHAR(UTF8Str)  (FStringConversion::Utf8ToTChar((const UTF8CHAR*)UTF8Str))

template<typename T>
class APRICOT_API TCStringCalls
{
public:
	using CharType = T;

public:
	static SizeType Strlen(const CharType* String);
};

using FCStringCalls     = TCStringCalls<TCHAR>;
using FCStringAnsiCalls = TCStringCalls<ANSICHAR>;
using FCStringUtf8Calls = TCStringCalls<UTF8CHAR>;

class APRICOT_API FStringConversion
{
public:
	static const ANSICHAR* TCharToAnsi(const TCHAR*    Str);
	static const TCHAR*    AnsiToTChar(const ANSICHAR* Str);
	static const UTF8CHAR* TCharToUtf8(const TCHAR*    Str);
	static const TCHAR*    Utf8ToTChar(const UTF8CHAR* Str);
};