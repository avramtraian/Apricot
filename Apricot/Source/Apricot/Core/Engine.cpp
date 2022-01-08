// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Engine.h"

#include "Memory.h"
#include "Profiler.h"
#include "Platform.h"

#include "Apricot/Filesystem/Filesystem.h"

namespace Apricot {

	Engine* GEngine = nullptr;

	Engine::Engine()
	{
		AE_PERFORMANCE_FUNCTION();
	}

	Engine::~Engine()
	{
		AE_PERFORMANCE_FUNCTION();
	}

	int32 Engine::Run(const char* commandArgs)
	{
		AE_PERFORMANCE_FUNCTION();

		GEngine = this;

		OnInitEngine();

		HTime lastFrameTime = Platform::GetPerformanceTime();
		while (m_bIsRunning)
		{
			AE_PERFORMANCE_BEGIN_FRAME("MainThread");

			HTime now = Platform::GetPerformanceTime();
			HTimestep timestep = now - lastFrameTime;
			lastFrameTime = now;
		}

		OnDestroyEngine();

		return 0;
	}

	void Engine::OnForceShutdown()
	{
		if (!m_bIsRunning)
		{
			return;
		}

		m_bIsRunning = false;
		Logger::Destroy();
		PerformanceProfiler::Destroy();
		Platform::Destroy();

		Memory::HMemoryDebugger::DebugLog();

		std::exit(-2);
	}

	bool Engine::OnInitEngine()
	{
		AE_PERFORMANCE_FUNCTION();

		AE_CORE_INFO("Engine::OnInitEngine()");
		AE_CORE_INFO("    Platform:      {}", AE_PLATFORM);
		AE_CORE_INFO("    Configuration: {}", AE_CONFIGURATION);
		AE_CORE_INFO("    Engine Type:   {}", AE_ENGINE_TYPE);

		// Core-Systems initialization
		{
			Filesystem::Init();
		}

		return true;
	}

	bool Engine::OnDestroyEngine()
	{
		AE_PERFORMANCE_FUNCTION();

		// Core-Systems destruction
		{
			Filesystem::Destroy();
		}

		AE_CORE_INFO("Engine::OnDestroyEngine()");
		Memory::HMemoryDebugger::DebugLog();
		return true;
	}

}