// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Engine.h"

#include "Memory.h"
#include "Profiler.h"
#include "Platform.h"

#include "Apricot/Filesystem/Filesystem.h"

#include "Apricot/Events/ApplicationEvents.h"
#include "Apricot/Events/WindowEvents.h"
#include "Apricot/Events/KeyboardEvents.h"
#include "Apricot/Events/MouseEvents.h"

namespace Apricot {

	Engine* GEngine = nullptr;

	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	int32 Engine::Run(const char8* commandArgs)
	{
		GEngine = this;

		OnInitEngine(commandArgs);

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

	void Engine::OnEvent(Event* e)
	{
		AE_CORE_TRACE("Event: {}", (uint16)e->GetType());

		static EventDispatchMap map;
		map.OnWindowClosed = [](const WindowClosedEvent* e) -> bool8 { return GEngine->OnWindowClosed(e); };
		map.OnWindowResized = [](const WindowResizedEvent* e) -> bool8 { return GEngine->OnWindowResized(e); };
		map.OnWindowMoved = [](const WindowMovedEvent* e) -> bool8 { return GEngine->OnWindowMoved(e); };

		map.OnKeyPressed = [](const KeyPressedEvent* e) -> bool8 { return GEngine->OnKeyPressed(e); };
		map.OnKeyReleased = [](const KeyReleasedEvent* e) -> bool8 { return GEngine->OnKeyReleased(e); };

		map.OnMouseMoved = [](const MouseMovedEvent* e) -> bool8 { return GEngine->OnMouseMoved(e); };
		map.OnMouseButtonPressed = [](const MouseButtonPressedEvent* e) -> bool8 { return GEngine->OnMouseButtonPressed(e); };
		map.OnMouseButtonReleased = [](const MouseButtonReleasedEvent* e) -> bool8 { return GEngine->OnMouseButtonReleased(e); };
		map.OnMouseWheelScrolled = [](const MouseWheelScrolledEvent* e) -> bool8 { return GEngine->OnMouseWheelScrolled(e); };

		EventDispatcher dispatcher(e, &map);
		dispatcher.Dispatch();
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

	bool Engine::OnInitEngine(const char8* commandLine)
	{
		Filesystem::Init();

		EngineConfig::Init(commandLine);
		Logger::Init();

		AE_CORE_INFO("Engine::OnInitEngine()");
		AE_CORE_INFO("    Platform:      {}", AE_PLATFORM);
		AE_CORE_INFO("    Configuration: {}", AE_CONFIGURATION);
		AE_CORE_INFO("    Engine Type:   {}", AE_ENGINE_TYPE);

		PerformanceProfiler::Init();

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

		PerformanceProfiler::Destroy();
		Logger::Destroy();
		EngineConfig::Destroy();

		return true;
	}

	bool8 Engine::OnWindowClosed(const WindowClosedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnWindowResized(const WindowResizedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnWindowMoved(const WindowMovedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnKeyPressed(const KeyPressedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnKeyReleased(const KeyReleasedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnMouseMoved(const MouseMovedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnMouseButtonPressed(const MouseButtonPressedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnMouseButtonReleased(const MouseButtonReleasedEvent* e)
	{
		return false;
	}

	bool8 Engine::OnMouseWheelScrolled(const MouseWheelScrolledEvent* e)
	{
		return false;
	}

}