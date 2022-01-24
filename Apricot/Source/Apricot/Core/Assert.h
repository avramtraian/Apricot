// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Log.h"

namespace Apricot {

	APRICOT_API void ReportBaseAssert(const char8* condition, const char8* file, uint64 line, const char8* func, const char8* message);
	APRICOT_API void ReportCoreAssert(const char8* condition, const char8* file, uint64 line, const char8* func, const char8* message);
	APRICOT_API void ReportBaseVerify(const char8* condition, const char8* file, uint64 line, const char8* func, const char8* message);
	APRICOT_API void ReportCoreVerify(const char8* condition, const char8* file, uint64 line, const char8* func, const char8* message);

}

#ifdef AE_ENABLE_ASSERTS
	#define AE_BASE_ASSERT(Condition, ...) if (!(Condition)) {                             AE_DEBUGBREAK(); ::Apricot::ReportBaseAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, ""##__VA_ARGS__); }
	#define AE_CORE_ASSERT(Condition, ...) if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); AE_DEBUGBREAK(); ::Apricot::ReportCoreAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, ""##__VA_ARGS__); }
#else
	#define AE_BASE_ASSERT(...)
	#define AE_CORE_ASSERT(...)
#endif

#ifdef AE_ENABLE_VERIFIES
	#define AE_BASE_VERIFY(Condition, ...) if (!(Condition)) {                             AE_DEBUGBREAK(); ::Apricot::ReportBaseVerify(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, ""##__VA_ARGS__); }
	#define AE_CORE_VERIFY(Condition, ...) if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); AE_DEBUGBREAK(); ::Apricot::ReportCoreVerify(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, ""##__VA_ARGS__); }
#else
	#define AE_BASE_VERIFY(...)
	#define AE_CORE_VERIFY(...)
#endif