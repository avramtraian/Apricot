#pragma once

#include "Engine.h"
#include "Memory.h"

#ifdef AE_PLATFORM_WINDOWS

#include <Windows.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	uint64 engineSize;
	Apricot::Engine* engine = Apricot::CreateEngine(engineSize);
	uint32 returnCode = engine->Run(lpCmdLine);
	Apricot::Memory::Delete(engine, engineSize, Apricot::Memory::AllocTag::CoreSystems);

	return (int)returnCode;
}

#endif