#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Compiler.h"

#if not PLATFORM_WINDOWS
	#error "Trying to compile a Windows only file!"
#else
	
	#include <Windows.h>
	#include <Windowsx.h>

	#if DEBUG_BUILD || RELEASE_BUILD
		#define WIN_CHECK_RESULT(Expression) if (!(Expression)) { Platform::WindowsReportError(GetLastError()); DEBUGBREAK(); }
	#else
		#define WIN_CHECK_RESULT(Expression) Expression
	#endif

#endif