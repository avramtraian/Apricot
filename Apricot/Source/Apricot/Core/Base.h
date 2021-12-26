#pragma once

/*
* Platform detection
*/
#ifdef AE_PLATFORM_WINDOWS
	#define AE_PLATFORM "Windows"
#else
	#pragma error "Apricot only supports Windows!"
#endif



/*
* APRICOT_API definition
*/
#ifdef AE_PLATFORM_WINDOWS
	#ifdef AE_EXPORT_DLL
		#define APRICOT_API __declspec(dllexport)
	#elif AE_IMPORT_DLL
		#define APRICOT_API __declspec(dllimport)
	#else
		#define APRICOT_API 
	#endif
#endif



/*
* Configuration detection
*/
#ifdef AE_CONFIG_DEBUG_GAME
	#define AE_DEBUG
	#define AE_GAME
	#define AE_CONFIGURATION "Debug"
#elif AE_CONFIG_RELEASE_GAME
	#define AE_RELEASE
	#define AE_GAME
	#define AE_CONFIGURATION "Release"
#elif AE_CONFIG_SHIPPING_GAME
	#define AE_SHIPPING
	#define AE_GAME
	#define AE_CONFIGURATION "Shipping"
#elif AE_CONFIG_DEBUG_EDITOR
	#define AE_DEBUG
	#define AE_EDITOR
	#define AE_CONFIGURATION "Debug"
#elif AE_CONFIG_RELEASE_EDITOR
	#define AE_RELEASE
	#define AE_EDITOR
	#define AE_CONFIGURATION "Release"
#else
	#pragma error "Unknown configuration macro!"
#endif

/*
* Debug settings
*/
#ifdef AE_DEBUG
	#define AE_ENABLE_LOG_TRACE
	#define AE_ENABLE_LOG_DEBUG
	#define AE_ENABLE_LOG_INFO
	#define AE_ENABLE_LOG_WARN
	#define AE_ENABLE_LOG_ERROR
	#define AE_ENABLE_LOG_FATAL
	#define AE_ENABLE_ASSERTS
	#define AE_ENABLE_VERIFIES
	#define AE_ENABLE_MEMORY_TRACE
#elif AE_RELEASE
	#define AE_ENABLE_LOG_INFO
	#define AE_ENABLE_LOG_WARN
	#define AE_ENABLE_LOG_ERROR
	#define AE_ENABLE_LOG_FATAL
	#define AE_ENABLE_VERIFIES
#endif



/*
* Compiler detection
*/
#ifdef _MSC_BUILD
	#define AE_COMPILER_MSVC
#else
	#pragma error "Unknown compiler!"
#endif
/*
* Language keywords
*/
#ifdef AE_COMPILER_MSVC
	#define STATIC_ASSERT(...) static_assert(__VA_ARGS__)
	#define DEBUGBREAK() __debugbreak();
#else
	#define STATIC_ASSERT(...) 
	#define DEBUGBREAK() 
#endif



/*
* Types
*/
#ifdef AE_COMPILER_MSVC
	using uint8		= unsigned char;
	using uint16	= unsigned short;
	using uint32	= unsigned int;
	using uint64	= unsigned long long;

	using int8		= signed char;
	using int16		= signed short;
	using int32		= signed int;
	using int64		= signed long long;

	using float32	= float;
	using float64	= double;

	using bool8		= bool;
	using bool32	= int;

	using char8		= char;
	using char16	= wchar_t;
#else
	// The compiler detection system will generate an error anyway
	// But we don't want 100's of compiler errors to deal with
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using int8 = signed char;
	using int16 = signed short;
	using int32 = signed int;
	using int64 = signed long long;

	using float32 = float;
	using float64 = double;

	using bool8 = bool;
	using bool32 = int;

	using char8 = char;
	using char16 = wchar_t;
#endif

STATIC_ASSERT(sizeof(uint8)		== 1, "sizeof(uint8) expected to be 8 bits!"	);
STATIC_ASSERT(sizeof(uint16)	== 2, "sizeof(uint16) expected to be 16 bits!"	);
STATIC_ASSERT(sizeof(uint32)	== 4, "sizeof(uint32) expected to be 32 bits!"	);
STATIC_ASSERT(sizeof(uint64)	== 8, "sizeof(uint64) expected to be 64 bits!"	);

STATIC_ASSERT(sizeof(int8)		== 1, "sizeof(int8) expected to be 8 bits!"		);
STATIC_ASSERT(sizeof(int16)		== 2, "sizeof(int16) expected to be 16 bits!"	);
STATIC_ASSERT(sizeof(int32)		== 4, "sizeof(int32) expected to be 32 bits!"	);
STATIC_ASSERT(sizeof(int64)		== 8, "sizeof(int64) expected to be 64 bits!"	);

STATIC_ASSERT(sizeof(float32)	== 4, "sizeof(float32) expected to be 32 bits!"	);
STATIC_ASSERT(sizeof(float64)	== 8, "sizeof(float64) expected to be 64 bits!"	);

STATIC_ASSERT(sizeof(bool8)		== 1, "sizeof(bool8) expected to be 8 bits!"	);
STATIC_ASSERT(sizeof(bool32)	== 4, "sizeof(bool32) expected to be 32 bits!"	);

STATIC_ASSERT(sizeof(char8)		== 1, "sizeof(char8) expected to be 8 bits!"	);
STATIC_ASSERT(sizeof(char16)	== 2, "sizeof(char16) expected to be 16 bits!"	);