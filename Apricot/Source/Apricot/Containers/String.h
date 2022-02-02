// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#ifdef TEXT
	#undef TEXT
#endif

#ifdef AE_ANSII
	using TChar = char8;
	#define TEXT(Text) Text
#elif AE_UNICODE
	using TChar = char16;
	#define TEXT(Text) L##Text
#else
	#error "No charset specified!"
#endif

namespace Apricot {

	APRICOT_API uint64 Str_Length(const char8* String);

	APRICOT_API uint64 Str_Length(const char16* String);

}