// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Log.h"
#include "CrashReporter.h"

#ifdef AE_ENABLE_DEBUG_CHECKS

	#define AE_DEBUG_CHECK(Expression)                                                                \
		if (!(Expression))                                                                            \
		{                                                                                             \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr);  \
			AE_DEBUGBREAK();                                                                          \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr); \
		}

	#define AE_DEBUG_CHECK_M(Expression, ...)                                                                                 \
		if (!(Expression))                                                                                                    \
		{                                                                                                                     \
			Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, ""##__VA_ARGS__);                    \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer);  \
			AE_DEBUGBREAK();                                                                                                  \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer); \
		}

	#define AE_DEBUG_CHECK_NO_ENTRY()                                                  \
		{                                                                              \
			GCrashReporter->PreCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                           \
			GCrashReporter->PostCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

	#define AE_DEBUG_CHECK_UNIMPLEMENTED()                                                   \
		{                                                                                    \
			GCrashReporter->PreCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                                 \
			GCrashReporter->PostCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

#else

	#define AE_DEBUG_CHECK(...)

	#define AE_DEBUG_CHECK_M(...)

	#define AE_DEBUG_CHECK_NO_ENTRY(...)

	#define AE_DEBUG_CHECK_UNIMPLEMENTED(...)

#endif


#ifdef AE_ENABLE_CHECKS

	#define AE_CHECK(Expression) \
		if (!(Expression))                                                                            \
		{                                                                                             \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr);  \
			AE_DEBUGBREAK();                                                                          \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr); \
		}
	
	#define AE_CHECK_M(Expression, ...) \
		if (!(Expression))                                                                                                    \
		{                                                                                                                     \
			Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, ""##__VA_ARGS__);                    \
			GCrashReporter->PreCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer);  \
			AE_DEBUGBREAK();                                                                                                  \
			GCrashReporter->PostCheckFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer); \
		}

	#define AE_CHECK_NO_ENTRY()                                                        \
		{                                                                              \
			GCrashReporter->PreCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                           \
			GCrashReporter->PostCheckNoEntryFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

	#define AE_CHECK_UNIMPLEMENTED()                                                         \
		{                                                                                    \
			GCrashReporter->PreCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE);  \
			AE_DEBUGBREAK();                                                                 \
			GCrashReporter->PostCheckUnimplementedFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE); \
		}

#else

	#define AE_CHECK(...)

	#define AE_CHECK_M(...)

	#define AE_CHECK_NO_ENTRY(...)

	#define AE_CHECK_UNIMPLEMENTED(...)

#endif


#ifdef AE_ENABLE_VERIFIES

#define AE_VERIFY(Expression)                                                                          \
		if (!(Expression))                                                                             \
		{                                                                                              \
			GCrashReporter->PreVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr);  \
			AE_DEBUGBREAK();                                                                           \
			GCrashReporter->PostVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr); \
		}

	#define AE_VERIFY_M(Expression, ...)                                                                                       \
		if (!(Expression))                                                                                                     \
		{                                                                                                                      \
			Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, ""##__VA_ARGS__);                     \
			GCrashReporter->PreVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer);  \
			AE_DEBUGBREAK();                                                                                                   \
			GCrashReporter->PostVerifyFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer); \
		}

#else

	#define AE_VERIFY(Expression)        { Expression; }

	#define AE_VERIFY_M(Expression, ...) { Expression; }

#endif


#ifdef AE_ENABLE_ENSURES

	#define AE_ENSURE(Expression)                                                                  \
		if (!(Expression))                                                                         \
		{                                                                                          \
			GCrashReporter->EnsureFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr); \
		}
										        
	#define AE_ENSURE_M(Expression, ...)                                                                                   \
		if (!(Expression))                                                                                                 \
		{                                                                                                                  \
			Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, ""##__VA_ARGS__);                 \
			GCrashReporter->EnsureFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer); \
		}
										        
	#define AE_ENSURE_ALWAYS(Expression)                                                                 \
		if (!(Expression))                                                                               \
		{                                                                                                \
			GCrashReporter->EnsureAlwaysFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, nullptr); \
		}

	#define AE_ENSURE_ALWAYS_M(Expression, ...)                                                                                  \
		if (!(Expression))                                                                                                       \
		{                                                                                                                        \
			Format(GCrashReporter->AssertionBuffer, GCrashReporter->AssertionBufferSize, ""##__VA_ARGS__);                       \
			GCrashReporter->EnsureAlwaysFailed(AE_FILE, AE_FUNCTION_SIG, AE_LINE, #Expression, GCrashReporter->AssertionBuffer); \
		}

#else

	#define AE_ENSURE(Expression)			    { Expression; }
											    
	#define AE_ENSURE_M(Expression, ...)	    { Expression; }
											    
	#define AE_ENSURE_ALWAYS(Expression, ...)   { Expression; }
											    
	#define AE_ENSURE_ALWAYS_M(Expression, ...) { Expression; }

#endif