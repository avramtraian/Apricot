// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API AEngine
	{
	public:
		int32 Run(const char8* CommandLine);

		void OnEvent(AEvent* e);

	private:
		bool8 OnEngineInitialize(const char8* CommandLine);
		bool8 OnEngineDestroy();

		bool8 OnWindowClosed(const AWindowClosedEvent* Event);
		bool8 OnWindowResized(const AWindowResizedEvent* Event);
		bool8 OnWindowMoved(const AWindowMovedEvent* Event);

		bool8 OnKeyPressed(const AKeyPressedEvent* Event);
		bool8 OnKeyReleased(const AKeyReleasedEvent* Event);

		bool8 OnMouseMoved(const AMouseMovedEvent* e);
		bool8 OnMouseButtonPressed(const AMouseButtonPressedEvent* Event);
		bool8 OnMouseButtonReleased(const AMouseButtonReleasedEvent* Event);
		bool8 OnMouseWheelScrolled(const AMouseWheelScrolledEvent* Event);

	private:
		AEventDispatchMap m_DispatchMap;
	};

	extern AEngine* CreateEngine();
	extern void DeleteEngine(AEngine* Engine);

	APRICOT_API extern AEngine* GEngine;

}