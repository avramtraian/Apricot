// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Engine.h"
#include "Memory.h"
#include "Platform.h"
#include "Log.h"
#include "Profiler.h"

#ifdef AE_PLATFORM_WINDOWS

#include <Windows.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Platform initialization.
	Apricot::Platform::Init();

	// Engine instantiation.
	uint64 engineSize;
	Apricot::Engine* engine = Apricot::CreateEngine(engineSize);
	uint32 returnCode = engine->Run(lpCmdLine);
	Apricot::Memory::Delete(engine, engineSize, Apricot::Memory::AllocTag::CoreSystems);

	// Platform destruction.
	Apricot::Platform::Destroy();

	// Application exit.
	return (int)returnCode;
}

#endif