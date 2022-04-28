#pragma once

#include "Base.h"
#include "Compiler.h"

#if PLATFORM_WINDOWS
	#if COMPILER_MSVC
		using uint8  = unsigned __int8;
		using uint16 = unsigned __int16;
		using uint32 = unsigned __int32;
		using uint64 = unsigned __int64;

		using int8   = signed __int8;
		using int16  = signed __int16;
		using int32  = signed __int32;
		using int64  = signed __int64;
	#endif
#endif

	#include <Windows.h>

	// An 8-bit character.
	using CHAR8      = char8_t;

	// A 16-bit character.
	using CHAR16     = char16_t;

	// An ANSI character. 8-bit fixed-width representation.
	using ANSICHAR   = char;

	// A ?-bit (depending on the platform) fixed-width representation.
	using WIDECHAR   = wchar_t;

	// An 8-bit character using the UTF8 - (Unicode, 8-bit, variable-width) - code unit.
	using UTF8CHAR   = CHAR8;

	// A 16-bit character using the UCS2 - (Unicode, 16-bit, fixed-width) - code unit.
	using UCS2CHAR   = CHAR16;

#if PLATFORM_ARCHITECTURE_32
	using SizeType = uint32;
	using PtrType = uint32;
#elif PLATFORM_ARCHITECTURE_64
	using SizeType = uint64;
	using PtrType = uint64;
#endif

typedef decltype(nullptr) NullptrType;

static_assert(sizeof(uint8)  == 1, "uint8 expected to be 8 bits!");
static_assert(sizeof(uint16) == 2, "uint16 expected to be 16 bits!");
static_assert(sizeof(uint32) == 4, "uint32 expected to be 32 bits!");
static_assert(sizeof(uint64) == 8, "uint64 expected to be 64 bits!");

static_assert(sizeof(int8)  == 1, "int8 expected to be 8 bits!");
static_assert(sizeof(int16) == 2, "int16 expected to be 16 bits!");
static_assert(sizeof(int32) == 4, "int32 expected to be 32 bits!");
static_assert(sizeof(int64) == 8, "int64 expected to be 64 bits!");

namespace AE {
	
	template<typename T>
	struct RemoveReference
	{
		using Type = T;
	};
	
	template<typename T>
	struct RemoveReference<T&>
	{
		using Type = T;
	};
	
	template<typename T>
	struct RemoveReference<T&&>
	{
		using Type = T;
	};
	
	template<typename T>
	using RemoveReferenceType = typename RemoveReference<T>::Type;
	
	template<typename T>
	constexpr RemoveReferenceType<T>&& Move(T&& Object) noexcept
	{
		return static_cast<RemoveReferenceType<T>&&>(Object);
	}
	
	template<typename T>
	constexpr T&& Forward(RemoveReferenceType<T>& Object) noexcept
	{
		return static_cast<T&&>(Object);
	}
	
}