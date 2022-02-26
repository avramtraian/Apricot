// Part of Apricot Engine. 2022-2022.
// Module: Core

#include "aepch.h"
#include "Engine.h"

#include "Platform.h"
#include "Memory/ApricotMemory.h"

#include "Apricot/Events/ApplicationEvents.h"
#include "Apricot/Events/WindowEvents.h"
#include "Apricot/Events/KeyboardEvents.h"
#include "Apricot/Events/MouseEvents.h"

#include "Apricot/Containers/Hash.h"

#include "Memory/PoolArena.h"
#include "Memory/LinearArena.h"
#include "Memory/StackArena.h"

namespace Apricot {

	AEngine* GEngine = nullptr;

	int32 AEngine::Run(const char8* CommandLine)
	{
		if (GEngine)
		{
			return AE_EXIT_UNKNOWN;
		}
		GEngine = this;

		if (!OnEngineInitialize(CommandLine))
		{
			return AE_EXIT_FAILED_INIT;
		}

		// AE_CORE_DEBUG(TEXT("{}"), AMemoryProfiler::GetRegionsUsageString());
		// AE_CORE_DEBUG(TEXT("{}"), AMemoryProfiler::GetSubregionsUsageString());
		// AE_CORE_DEBUG(TEXT("{}"), AMemoryProfiler::GetTaggedUsageString());

		while (true)
		{
			
		}

		if (!OnEngineDestroy())
		{
			return AE_EXIT_FAILED_DESTROY;
		}

		return AE_EXIT_SUCCESS;
	}

	void AEngine::OnEvent(AEvent* Event)
	{
		AEventDispatcher Dispatcher(Event, &m_DispatchMap);
		Dispatcher.Dispatch();
	}

	bool8 AEngine::OnEngineInitialize(const char8* CommandLine)
	{
		m_DispatchMap.OnWindowClosed = [](const AWindowClosedEvent* Ev) -> bool8 { return GEngine->OnWindowClosed(Ev); };
		m_DispatchMap.OnWindowResized = [](const AWindowResizedEvent* Ev) -> bool8 { return GEngine->OnWindowResized(Ev); };
		m_DispatchMap.OnWindowMoved = [](const AWindowMovedEvent* Ev) -> bool8 { return GEngine->OnWindowMoved(Ev); };
		m_DispatchMap.OnKeyPressed = [](const AKeyPressedEvent* Ev) -> bool8 { return GEngine->OnKeyPressed(Ev); };
		m_DispatchMap.OnKeyReleased = [](const AKeyReleasedEvent* Ev) -> bool8 { return GEngine->OnKeyReleased(Ev); };
		m_DispatchMap.OnMouseMoved = [](const AMouseMovedEvent* Ev) -> bool8 { return GEngine->OnMouseMoved(Ev); };
		m_DispatchMap.OnMouseButtonPressed = [](const AMouseButtonPressedEvent* Ev) -> bool8 { return GEngine->OnMouseButtonPressed(Ev); };
		m_DispatchMap.OnMouseButtonReleased = [](const AMouseButtonReleasedEvent* Ev) -> bool8 { return GEngine->OnMouseButtonReleased(Ev); };
		m_DispatchMap.OnMouseWheelScrolled = [](const AMouseWheelScrolledEvent* Ev) -> bool8 { return GEngine->OnMouseWheelScrolled(Ev); };

		ALogger::Init();

		AE_CORE_INFO(TEXT("Engine initialization succeded!"));
		AE_CORE_INFO(TEXT("Instance Info:"));
		AE_CORE_INFO(TEXT("    Platform:      {}"), AE_PLATFORM);
		AE_CORE_INFO(TEXT("    Configuration: {}"), AE_CONFIGURATION);
		AE_CORE_INFO(TEXT("    Instance Type: {}"), AE_ENGINE_TYPE);

		return true;
	}

	bool8 AEngine::OnEngineDestroy()
	{
		ALogger::Destroy();

		return true;
	}

	bool8 AEngine::OnWindowClosed(const AWindowClosedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnWindowResized(const AWindowResizedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnWindowMoved(const AWindowMovedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnKeyPressed(const AKeyPressedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnKeyReleased(const AKeyReleasedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnMouseMoved(const AMouseMovedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnMouseButtonPressed(const AMouseButtonPressedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnMouseButtonReleased(const AMouseButtonReleasedEvent* Event)
	{
		return false;
	}

	bool8 AEngine::OnMouseWheelScrolled(const AMouseWheelScrolledEvent* Event)
	{
		return false;
	}

}