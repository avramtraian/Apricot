#pragma once

#include "Application.h"
#include "Platform.h"

namespace Apricot {

	int32 ApricotMain(int argc, char** argv)
	{
		Platform::Init();

		Application* application = CreateApplication();
		int32 exitCode = application->Run(argc, argv);
		delete application;

		Platform::Destroy();

		return exitCode;
	}

}

#if defined (AE_SHIPPING) && defined (AE_PLATFORM_WINDOWS)

#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	return (INT)Apricot::ApricotMain(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return (int)Apricot::ApricotMain(argc, argv);
}

#endif