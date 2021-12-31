#pragma once

#include "Base.h"
#include "Log.h"

namespace Apricot {

	class APRICOT_API Assert
	{
	public:
		static void OnFatalAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message);

		template<typename... Args>
		static void OnFatalAssert(const char8* condition, const char8* file, uint64 line, const char8* funcSig, const char8* message, Args&&... args)
		{
			Memory::LinearAllocator allocator = Memory::LinearAllocator(8192);

			THFormatter<char8>* formatters = (THFormatter<char8>*)allocator.Allocate(AEC_MAX_LOG_ARGUMENT_COUNT * sizeof(THFormatter<char8>));
			for (uint64 index = 0; index < AEC_MAX_LOG_ARGUMENT_COUNT; index++)
			{
				Memory::PlacementNew<THFormatter<char8>>(formatters + index, &allocator);
			}

			uint64 argumentCount = 0;
			const char8* formatted = RawStringFormatter::Format(allocator, formatters, AEC_MAX_LOG_ARGUMENT_COUNT, argumentCount, message, std::forward<Args>(args)...);
			OnFatalAssert(condition, file, line, funcSig, formatted);
		}
	};

}

#ifdef AE_ENABLE_ASSERTS
	#define AE_CORE_ASSERT(Condition, ...)                     if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); Apricot::Assert::OnFatalAssert(#Condition, FILE_MACRO, LINE_MACRO, FUNCTION_SIG_MACRO, __VA_ARGS__); }
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...) if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_ASSERT_BREAK(Condition, ...)               if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); break; }
	#define AE_CORE_ASSERT_CONTINUE(Condition, ...)            if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); continue; }
#else
	#define AE_CORE_ASSERT(Condition, ...) 
	#define AE_CORE_ASSERT_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_ASSERT_BREAK(Condition, ...) 
	#define AE_CORE_ASSERT_CONTINUE(Condition, ...) 
#endif

#ifdef AE_ENABLE_VERIFIES
	#define AE_CORE_VERIFY(Condition, ...)                     if (!(Condition)) { AE_CORE_FATAL(__VA_ARGS__); DEBUGBREAK(); Apricot::Assert::OnFatalAssert(#Condition, FILE_MACRO, LINE_MACRO, FUNCTION_SIG_MACRO, __VA_ARGS__); }
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...) if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); return ReturnValue; }
	#define AE_CORE_VERIFY_BREAK(Condition, ...)               if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); break; }
	#define AE_CORE_VERIFY_CONTINUE(Condition, ...)            if (!(Condition)) { AE_CORE_ERROR(__VA_ARGS__); DEBUGBREAK(); continue; }
#else
	#define AE_CORE_VERIFY(Condition, ...) 
	#define AE_CORE_VERIFY_RETURN(Condition, ReturnValue, ...) 
	#define AE_CORE_VERIFY_BREAK(Condition, ...) 
	#define AE_CORE_VERIFY_CONTINUE(Condition, ...) 
#endif

#define RETURN_VOID 