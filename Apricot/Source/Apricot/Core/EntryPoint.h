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
	Apricot::Platform::Init();
	Apricot::Logger::Init();
	Apricot::PerformanceProfiler::Init();

	uint64 engineSize;
	Apricot::Engine* engine = Apricot::CreateEngine(engineSize);
	uint32 returnCode = engine->Run(lpCmdLine);
	Apricot::Memory::Delete(engine, engineSize, Apricot::Memory::AllocTag::CoreSystems);

	Apricot::PerformanceProfiler::Destroy();
	Apricot::Logger::Destroy();
	Apricot::Platform::Destroy();

	return (int)returnCode;
}

#endif