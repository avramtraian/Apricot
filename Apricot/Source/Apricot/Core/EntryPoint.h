// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Platform.h"
#include "CrashReporter.h"
#include "Engine.h"
#include "Memory/Memory.h"

#ifdef AE_PLATFORM_WINDOWS

#ifdef TEXT
	#undef TEXT
#endif

#include <Windows.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Init foundational systems.
	Apricot::APlatform::Init();
	Apricot::Memory_Init();
	Apricot::ACrashReporter::Init();

	// Instantiate the engine.
	Apricot::AEngine* Engine = Apricot::CreateEngine();

	// Run the engine.
	int32 ReturnCode = Engine->Run(lpCmdLine);

	// TODO: Dump debug information (to files?).
	switch (ReturnCode)
	{
		case AE_EXIT_SUCCESS:
		{
			break;
		}
		case AE_EXIT_UNKNOWN:
		{
			break;
		}
		case AE_EXIT_FAILED_INIT:
		{
			break;
		}
		case AE_EXIT_FAILED_DESTROY:
		{
			break;
		}
	}

	// Delete the engine.
	Apricot::DeleteEngine(Engine);

	// Destroy foundational systems.
	Apricot::ACrashReporter::Destroy();
	Apricot::Memory_Destroy();
	Apricot::APlatform::Destroy();

	return (int)ReturnCode;
}

#endif