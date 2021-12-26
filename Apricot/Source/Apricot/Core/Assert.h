#pragma once

#include "Base.h"
#include "Log.h"

#ifdef AE_ENABLE_ASSERTS
	#define AE_CORE_ASSERT(Condition, ...) if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); }
#endif

#ifdef AE_ENABLE_VERIFIES
	#define AE_CORE_VERIFY(Condition, ...) if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); }
#endif