#pragma once

#include "Base.h"
#include "Compiler.h"
#include "CrashReporter.h"

#ifdef DEBUG_BUILD
	#define COMPILER_WITH_ASSERTS  (1)
	#define COMPILER_WITH_VERIFIES (1)
#endif // AE_DEBUG

#ifdef RELEASE_BUILD
	#define COMPILER_WITH_ASSERTS  (1)
	#define COMPILER_WITH_VERIFIES (1)
#endif // AE_RELEASE

#ifdef SHIPPING_BUILD
	
#endif // AE_SHIPPING

#ifndef COMPILER_WITH_ASSERTS
	#define COMPILER_WITH_ASSERTS (0)
#endif

#ifndef COMPILER_WITH_VERIFIES
	#define COMPILER_WITH_VERIFIES (0)
#endif

#if COMPILER_WITH_ASSERTS

	#define AE_CORE_ASSERT(Expression)                                             \
		if (!(Expression))                                                         \
		{                                                                          \
			Apricot::CrashReporter::OnCoreAssert(#Expression, __FILE__, __LINE__); \
			DEBUGBREAK();                                                       \
		}

#else

	#define AE_CORE_ASSERT(Expression)

#endif // AE_ENABLE_ASSERTS

#if COMPILER_WITH_VERIFIES

	#define AE_CORE_VERIFY(Expression) \
		if (!(Expression)) \
		{ \
			Apricot::CrashReporter::OnCoreAssert(#Expression, __FILE__, __LINE__); \
			DEBUGBREAK(); \
		}

#else

	#define AE_CORE_VERIFY(Expression) Expression

#endif // AE_ENABLE_VERIFIES