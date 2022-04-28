#pragma once

#include "Application.h"
#include "Core.h"
#include "Platform.h"
#include "Memory.h"

#include "CString.h"

namespace Apricot {

	int32 ApricotMain(int argc, char** argv)
	{
		Application* application = CreateApplication();
		int32 exitCode = application->Run(argc, argv);
		adelete application;

		Platform::Destroy();
		return exitCode;
	}

}

#if AE_PLATFORM_WINDOWS

#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Apricot::Platform::InitWindows(hInstance);
	return (INT)Apricot::ApricotMain(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return (int)Apricot::ApricotMain(argc, argv);
}

#endif