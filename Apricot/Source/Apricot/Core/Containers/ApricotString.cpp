#include "aepch.h"
#include "ApricotString.h"

#include "Apricot/Core/CString.h"

namespace Apricot {

	FString::FString()
	{
		m_Elements.SetSizeUninitialized(1);
		m_Elements[0] = 0;
	}

	FString::FString(const FString& Other)
		: m_Elements(Other.m_Elements)
	{
	}

	FString::FString(FString&& Other) noexcept
		: m_Elements(AE::Move(Other.m_Elements))
	{
	}

	FString::FString(const ANSICHAR* Str)
	{
		SizeType StrSize = FCStringAnsiCalls::Strlen(Str) + 1;
		m_Elements.SetSizeUninitialized(StrSize);
		for (SizeType Index = 0; Index < StrSize; Index++)
		{
			m_Elements.Data()[Index] = (TCHAR)(Str[Index]);
		}
	}

	FString::FString(const WIDECHAR* Str)
	{
		SizeType StrSize = FCStringCalls::Strlen(Str) + 1;
		m_Elements.SetSizeUninitialized(StrSize);
		MemCpy(m_Elements.Data(), Str, StrSize * sizeof(WIDECHAR));
	}

	FString::FString(const UTF8CHAR* Str)
	{
		// TODO: Implement!
		DEBUGBREAK();
	}

	FString::FString(const UCS2CHAR* Str)
	{
		SizeType StrSize = FCStringCalls::Strlen((TCHAR*)Str) + 1;
		m_Elements.SetSizeUninitialized(StrSize);
		MemCpy(m_Elements.Data(), Str, StrSize * sizeof(UCS2CHAR));
	}

	FString::FString(SizeType StrLength, const ANSICHAR* Str)
	{
		m_Elements.SetSizeUninitialized(StrLength + 1);
		for (SizeType Index = 0; Index < (StrLength + 1); Index++)
		{
			m_Elements[Index] = (TCHAR)(Str[Index]);
		}
	}

	FString::FString(SizeType StrLength, const WIDECHAR* Str)
	{
		m_Elements.SetSizeUninitialized(StrLength + 1);
		MemCpy(m_Elements.Data(), Str, (StrLength + 1) * sizeof(WIDECHAR));
	}

	FString::FString(SizeType StrLength, const UTF8CHAR* Str)
	{
		// TODO: Implement!
		DEBUGBREAK();
	}

	FString::FString(SizeType StrLength, const UCS2CHAR* Str)
	{
		m_Elements.SetSizeUninitialized(StrLength + 1);
		MemCpy(m_Elements.Data(), Str, (StrLength + 1) * sizeof(UCS2CHAR));
	}

	FString::~FString()
	{
		m_Elements.SetSizeInternal(0);
	}

	FString& FString::operator=(const UCS2CHAR* Str)
	{
		SizeType StrSize = FCStringCalls::Strlen((TCHAR*)Str) + 1;
		m_Elements.SetSizeUninitialized(StrSize, false);
		return *this;
	}

	FString& FString::operator=(const UTF8CHAR* Str)
	{
		// TODO: Implement!
		DEBUGBREAK();
		return *this;
	}

	FString& FString::operator=(const WIDECHAR* Str)
	{
		SizeType StrSize = FCStringCalls::Strlen((TCHAR*)Str) + 1;
		m_Elements.SetSizeInternal(0);
		m_Elements.SetSizeUninitialized(StrSize, false);
		MemCpy(m_Elements.Data(), Str, StrSize * sizeof(WIDECHAR));
		return *this;
	}

	FString& FString::operator=(const ANSICHAR* Str)
	{
		SizeType StrSize = FCStringCalls::Strlen((TCHAR*)Str) + 1;
		m_Elements.SetSizeInternal(0);
		m_Elements.SetSizeUninitialized(StrSize, false);
		for (SizeType Index = 0; Index < StrSize; Index++)
		{
			m_Elements[Index] = (TCHAR)(Str[Index]);
		}
		return *this;
	}

	FString& FString::operator=(FString&& Other) noexcept
	{
		m_Elements.SetSizeInternal(0);
		m_Elements = AE::Move(Other.m_Elements);
		return *this;
	}

	FString& FString::operator=(const FString& Other)
	{
		m_Elements.SetSizeInternal(0);
		m_Elements = Other.m_Elements;
		return *this;
	}

}