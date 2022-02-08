// Part of Apricot Engine. 2022-2022.

#pragma once

/*
* Platform detection
*/
#ifdef AE_PLATFORM_WINDOWS
#else
	#error "Apricot only supports Windows!"
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
* Platform specific defines.
*/
#ifdef AE_PLATFORM_WINDOWS
	#define AE_VULKAN
	#define AE_D3D12
	#define AE_D3D11
	#define AE_OPENGL
#endif



/*
* Configuration detection
*/
#ifdef AE_CONFIG_DEBUG_GAME
	#define AE_DEBUG
	#define AE_GAME
#elif AE_CONFIG_RELEASE_GAME
	#define AE_RELEASE
	#define AE_GAME
#elif AE_CONFIG_SHIPPING_GAME
	#define AE_SHIPPING
	#define AE_GAME
#elif AE_CONFIG_DEBUG_EDITOR
	#define AE_DEBUG
	#define AE_EDITOR
#elif AE_CONFIG_RELEASE_EDITOR
	#define AE_RELEASE
	#define AE_EDITOR
#else
	#error "Unknown configuration macro!"
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
	#define AE_ENABLE_DEBUG_CHECKS
	#define AE_ENABLE_CHECKS
	#define AE_ENABLE_VERIFIES
	#define AE_ENABLE_ENSURES
	#define AE_ENABLE_MEMORY_TRACE
	#define AE_ENABLE_PERFORMANCE_PROFILING
	#define AE_ENABLE_FILESYSTEM_ERROR_CHECK
#elif defined(AE_RELEASE)
	#define AE_ENABLE_LOG_INFO
	#define AE_ENABLE_LOG_WARN
	#define AE_ENABLE_LOG_ERROR
	#define AE_ENABLE_LOG_FATAL
	#define AE_ENABLE_CHECKS
	#define AE_ENABLE_VERIFIES
	#define AE_ENABLE_ENSURES
	#define AE_ENABLE_FILESYSTEM_ERROR_CHECK
#endif



/*
* Compiler detection
*/
#ifdef _MSC_BUILD
	#define AE_COMPILER_MSVC
#else
	#error "Unknown compiler!"
#endif
/*
* Compiler keywords
*/
#ifdef AE_COMPILER_MSVC
	#define AE_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
	#define AE_DEBUGBREAK() __debugbreak()
	#define AE_LINE __LINE__

	#define NODISCARD [[nodiscard]]
	#define FORCEINLINE __forceinline
#else
	#define AE_STATIC_ASSERT(...)
	#define AE_DEBUGBREAK()
	#define AE_LINE
	#define AE_FILE
	#define AE_FUNCTION
	#define AE_FUNCTION_SIG
	#define NODISCARD
	#define FORCEINLINE
#endif

#define AE_EXIT_UNKNOWN        -1
#define AE_EXIT_FAILED_INIT    -2
#define AE_EXIT_FAILED_DESTROY -3
#define AE_EXIT_SUCCESS         0



#define AE_ARRAY_LENGTH(Array) (sizeof(Array) / sizeof(Array[0]))



/* Disables dll-interface compiler warning. It is usually generated around templates. */
#pragma warning (disable: 4251)