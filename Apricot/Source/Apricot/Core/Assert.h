// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Log.h"
#include "CrashReporter.h"

#ifdef AE_ENABLE_CORE_ASSERTS

	#define AE_CORE_ASSERT(Expression, ...)                                                                                                               \
		if (!(Expression))                                                                                                                                \
		{                                                                                                                                                 \
			bool formatted = Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, TEXT("")##__VA_ARGS__);                         \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr);  \
			AE_DEBUGBREAK();                                                                                                                              \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr); \
		}

	#define AE_CORE_ASSERT_NO_ENTRY()                                                  \
		{                                                                              \
			GCrashReporter->PreCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                           \
			GCrashReporter->PostCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

	#define AE_CORE_ASSERT_UNIMPLEMENTED()                                                   \
		{                                                                                    \
			GCrashReporter->PreCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                                 \
			GCrashReporter->PostCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

#else

	#define AE_CORE_ASSERT(...)

	#define AE_CORE_ASSERT_NO_ENTRY(...)

	#define AE_CORE_ASSERT_UNIMPLEMENTED(...)

#endif


#ifdef AE_ENABLE_CORE_RELEASE_ASSERTS

	#define AE_CORE_RASSERT(Expression, ...) \
		if (!(Expression))                                                                                                                                \
		{                                                                                                                                                 \
			bool formatted = Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, TEXT("")##__VA_ARGS__);                         \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr);  \
			AE_DEBUGBREAK();                                                                                                                              \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr); \
		}

	#define AE_CORE_RASSERT_NO_ENTRY()                                                 \
		{                                                                              \
			GCrashReporter->PreCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                           \
			GCrashReporter->PostCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

	#define AE_CORE_RASSERT_UNIMPLEMENTED()                                                  \
		{                                                                                    \
			GCrashReporter->PreCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                                 \
			GCrashReporter->PostCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

#else

	#define AE_CORE_RASSERT(...)

	#define AE_CORE_RASSERT_NO_ENTRY(...)

	#define AE_CORE_RASSERT_UNIMPLEMENTED(...)

#endif


#ifdef AE_ENABLE_CORE_VERIFIES

	#define AE_CORE_VERIFY(Expression, ...)                                                                                                                \
		if (!(Expression))                                                                                                                                 \
		{                                                                                                                                                  \
			bool formatted = Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, TEXT("")##__VA_ARGS__);                          \
			GCrashReporter->PreVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr);  \
			AE_DEBUGBREAK();                                                                                                                               \
			GCrashReporter->PostVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr); \
		}

#else

	#define AE_CORE_VERIFY(Expression)        { Expression; }

#endif


#ifdef AE_ENABLE_ENSURES
										        
	#define AE_ENSURE(Expression, ...)                                                                                                                 \
		if (!(Expression))                                                                                                                             \
		{                                                                                                                                              \
			bool formatted = Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, TEXT("")##__VA_ARGS__);                      \
			GCrashReporter->EnsureFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr); \
		}

	#define AE_ENSURE_ALWAYS(Expression, ...)                                                                                                                \
		if (!(Expression))                                                                                                                                   \
		{                                                                                                                                                    \
			bool formatted = Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize,  TEXT("")##__VA_ARGS__);                           \
			GCrashReporter->EnsureAlwaysFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, TEXT(#Expression), formatted ? GCrashReporter->AssertionBuffer : nullptr); \
		}

#else

	#define AE_ENSURE(Expression)			    { Expression; }
											    
	#define AE_ENSURE_ALWAYS(Expression, ...)   { Expression; }

#endif