#pragma once

#include "Base.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API Engine
	{
	public:
		int32 Run(const char8* commandLine);

		void OnEvent(Event* e);

	private:
		bool8 OnEngineInitialize(const char8* commandLine);
		bool8 OnEngineDestroy();

		bool8 OnWindowClosed(const WindowClosedEvent* e);
		bool8 OnWindowResized(const WindowResizedEvent* e);
		bool8 OnWindowMoved(const WindowMovedEvent* e);

		bool8 OnKeyPressed(const KeyPressedEvent* e);
		bool8 OnKeyReleased(const KeyReleasedEvent* e);

		bool8 OnMouseMoved(const MouseMovedEvent* e);
		bool8 OnMouseButtonPressed(const MouseButtonPressedEvent* e);
		bool8 OnMouseButtonReleased(const MouseButtonReleasedEvent* e);
		bool8 OnMouseWheelScrolled(const MouseWheelScrolledEvent* e);
	};

	extern Engine* CreateEngine();
	extern void DeleteEngine(Engine* engine);

	extern Engine* GEngine;

}