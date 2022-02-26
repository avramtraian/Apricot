// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#include <type_traits>

#ifdef AE_COMPILER_MSVC

	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using int8 = signed char;
	using int16 = signed short;
	using int32 = signed int;
	using int64 = signed long long;

	/* Currently, Apricot engine runs ONLY on x64 computers. So we don't need to worry */
/* #ifdef AE_ARCHITECTURE_X64 */
	using uintptr = uint64;
/* #endif */

	using float32 = float;
	using float64 = double;

	using bool8 = bool;
	using bool32 = int;

	using char8 = char;
	using char16 = wchar_t;

#else

	#include <stdint.h>

	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;

	using uintptr = uintptr_t;

	using float32 = float;
	using float64 = double;

	using bool8 = bool;
	using bool32 = int;

	using char8 = char;
	using char16 = wchar_t;

#endif

AE_STATIC_ASSERT(sizeof(uint8)   == 1, "sizeof(uint8) expected to be 8 bits!");
AE_STATIC_ASSERT(sizeof(uint16)  == 2, "sizeof(uint16) expected to be 16 bits!");
AE_STATIC_ASSERT(sizeof(uint32)  == 4, "sizeof(uint32) expected to be 32 bits!");
AE_STATIC_ASSERT(sizeof(uint64)  == 8, "sizeof(uint64) expected to be 64 bits!");
								 
AE_STATIC_ASSERT(sizeof(int8)    == 1, "sizeof(int8) expected to be 8 bits!");
AE_STATIC_ASSERT(sizeof(int16)   == 2, "sizeof(int16) expected to be 16 bits!");
AE_STATIC_ASSERT(sizeof(int32)   == 4, "sizeof(int32) expected to be 32 bits!");
AE_STATIC_ASSERT(sizeof(int64)   == 8, "sizeof(int64) expected to be 64 bits!");

AE_STATIC_ASSERT(sizeof(uintptr) == sizeof(void*), "sizeof(uintptr) expected to be sizeof(void*)!");

AE_STATIC_ASSERT(sizeof(float32) == 4, "sizeof(float32) expected to be 32 bits!");
AE_STATIC_ASSERT(sizeof(float64) == 8, "sizeof(float64) expected to be 64 bits!");

AE_STATIC_ASSERT(sizeof(bool8)   == 1, "sizeof(bool8) expected to be 8 bits!");
AE_STATIC_ASSERT(sizeof(bool32)  == 4, "sizeof(bool32) expected to be 32 bits!");
								 
AE_STATIC_ASSERT(sizeof(char8)   == 1, "sizeof(TChar) expected to be 8 bits!");
AE_STATIC_ASSERT(sizeof(char16)  == 2, "sizeof(char16) expected to be 16 bits!");

#define AE_INT8_MIN   (-127i8 - 1)
#define AE_INT16_MIN  (-32767i16 - 1)
#define AE_INT32_MIN  (-2147483647i32 - 1)
#define AE_INT64_MIN  (-9223372036854775807i64 - 1)
#define AE_INT8_MAX   127i8
#define AE_INT16_MAX  32767i16
#define AE_INT32_MAX  2147483647i32
#define AE_INT64_MAX  9223372036854775807i64
#define AE_UINT8_MAX  0xffui8
#define AE_UINT16_MAX 0xffffui16
#define AE_UINT32_MAX 0xffffffffui32
#define AE_UINT64_MAX 0xffffffffffffffffui64

namespace Apricot {

	template<typename T>
	struct TRemoveReference
	{
		using Type = T;
	};

	template<typename T>
	struct TRemoveReference<T&>
	{
		using Type = T;
	};

	template<typename T>
	struct TRemoveReference<T&&>
	{
		using Type = T;
	};

	template<typename T>
	using TRemoveReference_Type = TRemoveReference<T>::Type;

	template<typename T>
	struct TRemoveConst
	{
		using Type = T;
	};

	template<typename T>
	struct TRemoveConst<const T>
	{
		using Type = T;
	};

	template<typename T>
	using TRemoveConst_Type = TRemoveConst<T>::Type;

	template<typename T>
	NODISCARD FORCEINLINE constexpr T&& Forward(TRemoveReference_Type<T>& Argument) noexcept
	{
		return static_cast<T&&>(Argument);
	}

	template<typename T>
	NODISCARD FORCEINLINE constexpr TRemoveReference_Type<T>&& Move(T&& Argument) noexcept
	{
		return static_cast<TRemoveReference_Type<T>&&>(Argument);
	}

	template<typename ClassType, typename BaseType>
	NODISCARD FORCEINLINE constexpr bool8 IsDerivedFrom() noexcept
	{
		return std::is_base_of<BaseType, ClassType>::value;
	}

	template<typename A, typename B>
	NODISCARD FORCEINLINE constexpr bool8 AreSameType() noexcept
	{
		return std::is_same<A, B>::value;
	}
	
}

#include "Char.h"


#ifdef AE_PLATFORM_WINDOWS
	#define AE_PLATFORM TEXT("Windows")
#endif

#ifdef AE_CONFIG_DEBUG_GAME

	#define AE_CONFIGURATION TEXT("Debug")
	#define AE_ENGINE_TYPE TEXT("Game")

#elif AE_CONFIG_RELEASE_GAME

	#define AE_CONFIGURATION TEXT("Release")
	#define AE_ENGINE_TYPE TEXT("Game")

#elif AE_CONFIG_SHIPPING_GAME

	#define AE_CONFIGURATION TEXT("Shipping")
	#define AE_ENGINE_TYPE TEXT("Game")

#elif AE_CONFIG_DEBUG_EDITOR

	#define AE_CONFIGURATION TEXT("Debug")
	#define AE_ENGINE_TYPE TEXT("Editor")

#elif AE_CONFIG_RELEASE_EDITOR

	#define AE_CONFIGURATION TEXT("Release")
	#define AE_ENGINE_TYPE TEXT("Editor")

#endif