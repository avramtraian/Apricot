// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#ifdef TEXT
	#undef TEXT
#endif

#ifdef AE_ANSII

	using TChar = char8;
	#define TEXT(Text) Text

	#ifdef AE_COMPILER_MSVC
		#define AE_FILE         __FILE__
		#define AE_FUNCTION     __FUNCTION__
		#define AE_FUNCTION_SIG __FUNCSIG__
	#endif

#elif AE_UNICODE

	using TChar = char16;
	
	#define _IMPL_TEXT(Text) L##Text
	#define TEXT(Text) _IMPL_TEXT(Text)

	#ifdef AE_COMPILER_MSVC
		#define AE_FILE         TEXT(__FILE__)
		#define AE_FUNCTION     TEXT(__FUNCTION__)
		#define AE_FUNCTION_SIG TEXT(__FUNCSIG__)
	#endif

#else
	#error "No charset specified!"
#endif