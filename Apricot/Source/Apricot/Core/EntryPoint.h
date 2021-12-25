#pragma once

#include "Engine.h"

#ifdef AE_PLATFORM_WINDOWS

#include <Windows.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Apricot::Engine* engine = Apricot::CreateEngine();
	uint32 returnCode = engine->Run(lpCmdLine);
	delete engine;

	return (int)returnCode;
}

#endif