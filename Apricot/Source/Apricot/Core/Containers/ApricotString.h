#pragma once

#include "Apricot/Core/Core.h"

#include "Vector.h"

namespace Apricot {

	class APRICOT_API FString
	{
	public:
		FString();
		FString(const FString& Other);
		FString(FString&& Other) noexcept;

		FString(const ANSICHAR* Str);
		FString(const WIDECHAR* Str);
		FString(const UTF8CHAR* Str);
		FString(const UCS2CHAR* Str);

		FString(SizeType StrLength, const ANSICHAR* Str);
		FString(SizeType StrLength, const WIDECHAR* Str);
		FString(SizeType StrLength, const UTF8CHAR* Str);
		FString(SizeType StrLength, const UCS2CHAR* Str);

		~FString();

	public:
		FString& operator=(const FString& Other);
		FString& operator=(FString&& Other) noexcept;

		FString& operator=(const ANSICHAR* Str);
		FString& operator=(const WIDECHAR* Str);
		FString& operator=(const UTF8CHAR* Str);
		FString& operator=(const UCS2CHAR* Str);

	public:
		FORCEINLINE SizeType Length() const { return m_Elements.Size() - 1; }
		FORCEINLINE SizeType MaxSize() const { return m_Elements.Capacity() - 1; }

		FORCEINLINE TCHAR* Data() const { return m_Elements.Data(); }
		FORCEINLINE const TCHAR* CStr() const { return m_Elements.Data(); }

	private:
		Vector<TCHAR> m_Elements;
	};

}