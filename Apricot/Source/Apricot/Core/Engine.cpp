// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Engine.h"

#include "Memory.h"
#include "Platform.h"

#include "Apricot/Events/ApplicationEvents.h"
#include "Apricot/Events/WindowEvents.h"
#include "Apricot/Events/KeyboardEvents.h"
#include "Apricot/Events/MouseEvents.h"

#include "Apricot/Containers/Vector.h"

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

		AE_CORE_DEBUG("{}", AMemoryDebugger::GetUsageString());

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
		static AEventDispatchMap Map;
		Map.OnWindowClosed = [](const AWindowClosedEvent* Ev) -> bool8 { return GEngine->OnWindowClosed(Ev); };
		Map.OnWindowResized = [](const AWindowResizedEvent* Ev) -> bool8 { return GEngine->OnWindowResized(Ev); };
		Map.OnWindowMoved = [](const AWindowMovedEvent* Ev) -> bool8 { return GEngine->OnWindowMoved(Ev); };

		Map.OnKeyPressed = [](const AKeyPressedEvent* Ev) -> bool8 { return GEngine->OnKeyPressed(Ev); };
		Map.OnKeyReleased = [](const AKeyReleasedEvent* Ev) -> bool8 { return GEngine->OnKeyReleased(Ev); };

		Map.OnMouseMoved = [](const AMouseMovedEvent* Ev) -> bool8 { return GEngine->OnMouseMoved(Ev); };
		Map.OnMouseButtonPressed = [](const AMouseButtonPressedEvent* Ev) -> bool8 { return GEngine->OnMouseButtonPressed(Ev); };
		Map.OnMouseButtonReleased = [](const AMouseButtonReleasedEvent* Ev) -> bool8 { return GEngine->OnMouseButtonReleased(Ev); };
		Map.OnMouseWheelScrolled = [](const AMouseWheelScrolledEvent* Ev) -> bool8 { return GEngine->OnMouseWheelScrolled(Ev); };

		AEventDispatcher Dispatcher(Event, &Map);
		Dispatcher.Dispatch();
	}

	bool8 AEngine::OnEngineInitialize(const char8* CommandLine)
	{
		ALogger::Init();

		AE_CORE_INFO("Engine initialization succeded!");
		AE_CORE_INFO("Instance Info:");
		AE_CORE_INFO("    Platform:      {}", AE_PLATFORM);
		AE_CORE_INFO("    Configuration: {}", AE_CONFIGURATION);
		AE_CORE_INFO("    Instance Type: {}", AE_ENGINE_TYPE);

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