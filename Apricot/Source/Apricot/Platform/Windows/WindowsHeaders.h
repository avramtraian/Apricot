#pragma once

#include "Apricot/Core/Base.h"

#ifndef AE_PLATFORM_WINDOWS
	#error "Trying to compile a Windows only file!"
#else
	
	#include <Windows.h>
	#include <Windowsx.h>

	#if defined(AE_DEBUG) || defined(AE_RELEASE)
		#define WIN_CHECK_RESULT(Expression) if (!(Expression)) { Platform::WindowsReportError(GetLastError()); AE_DEBUGBREAK(); }
	#else
		#define WIN_CHECK_RESULT(Expression) Expression
	#endif

#endif