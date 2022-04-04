#pragma once

#ifdef AE_COMPILER_MSVC
	#define AE_DEBUGBREAK() __debugbreak()

	#define COMPILER_DISABLE_WARNING(WarningID) warning (disable : WarningID)
#else
	#error "Unknown or unsupported compiler!"

	#define AE_DEBUGBREAK()
	#define COMPILER_DISABLE_WARNING()
#endif