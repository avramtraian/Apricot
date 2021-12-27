#pragma once

#include "Base.h"
#include "Log.h"

#ifdef AE_ENABLE_ASSERTS
	#define AE_CORE_ASSERT(Condition, ...)						if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); }
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...)	if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_ASSERT_BREAK(Condition, ...)	if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); break; }
#else
	#define AE_CORE_ASSERT(Condition, ...) 
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_ASSERT_BREAK(Condition, ...) 
#endif

#ifdef AE_ENABLE_VERIFIES
	#define AE_CORE_VERIFY(Condition, ...)						if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); }
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...)	if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_VERIFY_BREAK(Condition, ...)	if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); break; }
#else
	#define AE_CORE_VERIFY(Condition, ...) 
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_VERIFY_BREAK(Condition, ...) 
#endif

#define RETURN_VOID 