// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Log.h"

namespace Apricot {

	APRICOT_API void OnCoreAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message);

	template<typename... Args>
	void OnCoreAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message, Args&&... args)
	{
		Memory::LinearAllocator allocator = Memory::LinearAllocator(8192);
		const char8* formatted = RawStringFormatter::Format(allocator, message, std::forward<Args>(args)...);

		OnCoreAssert(condition, file, line, funcSig, formatted);
	}

	APRICOT_API void OnBaseAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message);

}

/*
* Assertion macro definitions.
*/
#ifdef AE_ENABLE_ASSERTS
	/*
	* BASE_ASSERTs can be called at ANY time.
	* They are used before initializing the major core-systems, such as logging.
	*/
	#define AE_BASE_ASSERT(Condition, Message)                     if (!(Condition)) { AE_DEBUGBREAK(); ::Apricot::OnBaseAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, Message); }
	#define AE_BASE_ASSERT_RETURN(Condition, ReturnValue, Message) if (!(Condition)) { AE_DEBUGBREAK(); return ReturnValue; }
	#define AE_BASE_ASSERT_BREAK(Condition, Message)               if (!(Condition)) { AE_DEBUGBREAK(); break; }
	#define AE_BASE_ASSERT_CONTINUE(Condition, Message)            if (!(Condition)) { AE_DEBUGBREAK(); continue; }

	/*
	* CORE_ASSERTs are the most common.
	* They provide much more info about the assertion, but the can't be called in the early stages of the engine lifetime.
	* Calling them to early will not cause a crash, but can be undefined behavior, as well as confusing others.
	*/
	#define AE_CORE_ASSERT(Condition, ...)                         if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); AE_DEBUGBREAK(); Apricot::OnCoreAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, __VA_ARGS__); }
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...)     if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_ASSERT_BREAK(Condition, ...)                   if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); break; }
	#define AE_CORE_ASSERT_CONTINUE(Condition, ...)                if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); continue; }
#else
	#define AE_BASE_ASSERT(Condition, Message) 
	#define AE_BASE_ASSERT_RETURN(Condition, ReturnValue, Message) 
	#define AE_BASE_ASSERT_BREAK(Condition, Message) 
	#define AE_BASE_ASSERT_CONTINUE(Condition, Message) 

	#define AE_CORE_ASSERT(Condition, ...) 
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_ASSERT_BREAK(Condition, ...) 
	#define AE_CORE_ASSERT_CONTINUE(Condition, ...) 
#endif

/*
* Verifies macro definitions.
* They are the same as the assertion macros, but they are included in release builds as well.
* BASE/CORE_VERIFYs serve the same purpose as the assertions.
*/
#ifdef AE_ENABLE_VERIFIES
	#define AE_BASE_VERIFY(Condition, Message)                     if (!(Condition)) { AE_DEBUGBREAK(); ::Apricot::OnBaseAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, Message); }
	#define AE_BASE_VERIFY_RETURN(Condition, ReturnValue, Message) if (!(Condition)) { AE_DEBUGBREAK(); return ReturnValue; }
	#define AE_BASE_VERIFY_BREAK(Condition, Message)               if (!(Condition)) { AE_DEBUGBREAK(); break; }
	#define AE_BASE_VERIFY_CONTINUE(Condition, Message)            if (!(Condition)) { AE_DEBUGBREAK(); continue; }

	#define AE_CORE_VERIFY(Condition, ...)                         if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); AE_DEBUGBREAK(); Apricot::OnCoreAssert(#Condition, AE_FILE, AE_LINE, AE_FUNCTION_SIG, __VA_ARGS__); }
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...)     if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_VERIFY_BREAK(Condition, ...)                   if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); break; }
	#define AE_CORE_VERIFY_CONTINUE(Condition, ...)                if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); AE_DEBUGBREAK(); continue; }
#else
	#define AE_BASE_VERIFY(Condition, Message) 
	#define AE_BASE_VERIFY_RETURN(Condition, ReturnValue, Message) 
	#define AE_BASE_VERIFY_BREAK(Condition, Message) 
	#define AE_BASE_VERIFY_CONTINUE(Condition, Message) 

	#define AE_CORE_VERIFY(Condition, ...) 
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_VERIFY_BREAK(Condition, ...) 
	#define AE_CORE_VERIFY_CONTINUE(Condition, ...) 
#endif

#define AE_VOID 