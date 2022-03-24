#pragma once



#ifdef AE_PLATFORM_WINDOWS
#else
	#error "Unknown or unsupported platform!"
	#error "Apricot currently only supports Windows!"
#endif



#ifdef AE_PLATFORM_WINDOWS
	
	#ifdef AE_EXPORT_CORE_LIBRARY
		#define APRICOT_API __declspec(dllexport)
	#elif AE_IMPORT_CORE_LIBRARY
		#define APRICOT_API __declspec(dllimport)
	#elif AE_IGNORE_CORE_LIBRARY
		#define APRICOT_API
	#else
		#error "You must specify AE_EXPORT_CORE_LIBRARY, AE_IMPORT_CORE_LIBRARY or AE_IGNORE_CORE_LIBRARY!"
		#define APRICOT_API
	#endif

#endif // AE_PLATFORM_WINDOWS



#ifdef AE_CONFIGURATION_EDITOR_DEBUG
	#define AE_EDITOR
	#define AE_DEBUG
	#define AE_EDITOR_DEBUG
#endif // AE_CONFIGURATION_EDITOR_DEBUG



#ifdef AE_CONFIGURATION_EDITOR_RELEASE
	#define AE_EDITOR
	#define AE_RELEASE
	#define AE_EDITOR_RELEASE
#endif // AE_CONFIGURATION_EDITOR_RELEASE



#ifdef AE_CONFIGURATION_DEBUG
	#define AE_DEBUG
#endif // AE_CONFIGURATION_DEBUG



#ifdef AE_CONFIGURATION_RELEASE
	#define AE_RELEASE
#endif // AE_CONFIGURATION_RELEASE



#ifdef AE_CONFIGURATION_SHIPPING
	#define AE_SHIPPING
#endif // AE_CONFIGURATION_SHIPPING



#ifdef _MSC_BUILD
	#define AE_COMPILER_MSVC
#else
	#error "Unknown or unsupported compiler!"
#endif

#include "Types.h"