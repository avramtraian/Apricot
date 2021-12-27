#include "aepch.h"
#include "Engine.h"

#include "Memory.h"

namespace Apricot {

	Engine* Engine::s_Engine = nullptr;

	Engine::Engine()
	{
		
	}

	Engine::~Engine()
	{

	}

	int32 Engine::Run(const char* commandArgs)
	{
		s_Engine = this;

		OnInitEngine();

		while (true)
		{

		}

		OnDestroyEngine();

		return 0;
	}

	bool Engine::OnInitEngine()
	{
		AE_CORE_INFO("Engine::OnInitEngine()");
		Memory::HMemoryDebugger::DebugLog();
		return true;
	}

	bool Engine::OnDestroyEngine()
	{
		AE_CORE_INFO("Engine::OnDestroyEngine()");
		Memory::HMemoryDebugger::DebugLog();
		return true;
	}

}