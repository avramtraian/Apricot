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

	int32 AEngine::Run(const char8* commandLine)
	{
		if (GEngine)
		{
			return AE_EXIT_UNKNOWN;
		}
		GEngine = this;

		if (!OnEngineInitialize(commandLine))
		{
			return AE_EXIT_FAILED_INIT;
		}

		AE_CORE_DEBUG("{}", MemoryDebugger::GetUsageString());

		TVector<uint32> buffer;
		buffer.SetAllocator(GAllocator);

		while (true)
		{
			
		}

		if (!OnEngineDestroy())
		{
			return AE_EXIT_FAILED_DESTROY;
		}

		return AE_EXIT_SUCCESS;
	}

	void AEngine::OnEvent(Event* e)
	{
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

	bool8 AEngine::OnEngineInitialize(const char8* commandLine)
	{
		ALogger::Init();

		return true;
	}

	bool8 AEngine::OnEngineDestroy()
	{
		ALogger::Destroy();

		return true;
	}

	bool8 AEngine::OnWindowClosed(const WindowClosedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnWindowResized(const WindowResizedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnWindowMoved(const WindowMovedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnKeyPressed(const KeyPressedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnKeyReleased(const KeyReleasedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnMouseMoved(const MouseMovedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnMouseButtonPressed(const MouseButtonPressedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnMouseButtonReleased(const MouseButtonReleasedEvent* e)
	{
		return false;
	}

	bool8 AEngine::OnMouseWheelScrolled(const MouseWheelScrolledEvent* e)
	{
		return false;
	}

}