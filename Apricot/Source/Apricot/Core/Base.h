#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS (1)
		#define PLATFORM_WIN64   (1)
	#else
		#define PLATFORM_WINDOWS (1)
		#define PLATFORM_WIN32   (1)
	#endif
#endif

#ifndef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS (0)
#endif
#ifndef PLATFORM_WIN32
	#define PLATFORM_WIN32 (0)
#endif
#ifndef PLATFORM_WIN64
	#define PLATFORM_WIN64 (0)
#endif

#if PLATFORM_WINDOWS
	#if PLATFORM_WIN32
		#define PLATFORM_ARCHITECTURE_32 (1)
	#elif PLATFORM_WIN64
		#define PLATFORM_ARCHITECTURE_64 (1)
	#else
		#error "Unknown Platform Architecture!"
	#endif
#endif

#ifndef PLATFORM_ARCHITECTURE_32
	#define PLATFORM_ARCHITECTURE_32 (0)
#endif
#ifndef PLATFORM_ARCHITECTURE_64
	#define PLATFORM_ARCHITECTURE_64 (0)
#endif

#if defined(AE_EXPORT_CORE_LIBRARY)
	#define EXPORT_CORE_LIBRARY (1)
#elif defined(AE_IMPORT_CORE_LIBRARY)
	#define IMPORT_CORE_LIBRARY (1)
#endif

#ifndef EXPORT_CORE_LIBRARY
	#define EXPORT_CORE_LIBRARY (0)
#endif

#ifndef IMPORT_CORE_LIBRARY
	#define IMPORT_CORE_LIBRARY (0)
#endif

#if PLATFORM_WINDOWS
	#if EXPORT_CORE_LIBRARY
		#define APRICOT_API __declspec(dllexport)
	#elif IMPORT_CORE_LIBRARY
		#define APRICOT_API __declspec(dllimport)
	#else
		#define APRICOT_API
	#endif
#endif

#ifdef AE_CONFIGURATION_EDITOR_DEBUG
	#define DEBUG_BUILD          (1)
	#define EDITOR_BUILD         (1)
	#define EDITOR_DEBUG_BUILD   (1)
#endif

#ifdef AE_CONFIGURATION_EDITOR_RELEASE
	#define RELEASE_BUILD        (1)
	#define EDITOR_BUILD         (1)
	#define EDITOR_RELEASE_BUILD (1)
#endif

#ifdef AE_CONFIGURATION_DEBUG
	#define DEBUG_BUILD          (1)
#endif

#ifdef AE_CONFIGURATION_RELEASE
	#define RELEASE_BUILD        (1)
#endif

#ifdef AE_CONFIGURATION_SHIPPING
	#define SHIPPING_BUILD       (1)
#endif

#ifndef DEBUG_BUILD
	#define DEBUG_BUILD (0)
#endif
#ifndef RELEASE_BUILD
	#define RELEASE_BUILD (0)
#endif
#ifndef SHIPPING_BUILD
	#define SHIPPING_BUILD (0)
#endif
#ifndef EDITOR_BUILD
	#define EDITOR_BUILD (0)
#endif
#ifndef EDITOR_DEBUG_BUILD
	#define EDITOR_DEBUG_BUILD (0)
#endif
#ifndef EDITOR_RELEASE_BUILD
	#define EDITOR_RELEASE_BUILD (0)
#endif

/// TEMP
#define ASTL_API APRICOT_API