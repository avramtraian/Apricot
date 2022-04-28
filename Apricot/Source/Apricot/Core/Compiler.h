#pragma once

#ifdef _MSC_BUILD
	#define COMPILER_MSVC (1)
#endif

#ifndef COMPILER_MSVC
	#define COMPILER_MSVC (0)
#endif

#if COMPILER_MSVC
	#define DEBUGBREAK() __debugbreak()
	#define FORCEINLINE __forceinline

	// 'operator new': non-member operator new or delete functions may not be declared inline
	#pragma warning(disable : 4595)

	// class 'C1' needs to have dll-interface to be used by clients of class 'C2'
	#pragma warning(disable : 4251)
#else
	#error "Unknown Compiler!"
#endif

#define NODISCARD    [[nodiscard]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY       [[likely]]
#define UNLIKELY     [[unlikely]]