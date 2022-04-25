#pragma once

#include "Base.h"
#include "CrashReporter.h"

#ifdef AE_DEBUG
	#define AE_ENABLE_ASSERTS
	#define AE_ENABLE_VERIFIES
#endif // AE_DEBUG

#ifdef AE_RELEASE
	#define AE_ENABLE_ASSERTS
	#define AE_ENABLE_VERIFIES
#endif // AE_RELEASE

#ifdef AE_SHIPPING
	
#endif // AE_SHIPPING

#ifdef AE_ENABLE_ASSERTS

	#define AE_CORE_ASSERT(Expression)                                             \
		if (!(Expression))                                                         \
		{                                                                          \
			Apricot::CrashReporter::OnCoreAssert(#Expression, __FILE__, __LINE__); \
			AE_DEBUGBREAK();                                                       \
		}

#else

	#define AE_CORE_ASSERT(Expression)

#endif // AE_ENABLE_ASSERTS

#ifdef AE_ENABLE_VERIFIES

	#define AE_CORE_VERIFY(Expression) \
		if (!(Expression)) \
		{ \
			Apricot::CrashReporter::OnCoreAssert(#Expression, __FILE__, __LINE__); \
			AE_DEBUGBREAK(); \
		}

#else

	#define AE_CORE_VERIFY(Expression) Expression

#endif // AE_ENABLE_VERIFIES