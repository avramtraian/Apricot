// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#include "LayerStack.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API AEngine
	{
	public:
		AEngine() = default;
		virtual ~AEngine() = default;

	public:
		int32 Run(const char8* CommandLine);

		void OnEvent(AEvent* e);

		template<typename T>
		void PushLayer()
		{
			T* layer = (T*)GMalloc->Alloc(sizeof(T));
			MemConstruct<T>(layer);
			m_LayerStack.PushLayer(layer);
		}

		template<typename T>
		void PushOverlay()
		{
			T* overlay = (T*)GMalloc->Alloc(sizeof(T));
			MemConstruct<T>(overlay);
			m_LayerStack.PushOverlay(overlay);
		}

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

	protected:
		LayerStack m_LayerStack;

		AEventDispatchMap m_DispatchMap;
	};

	extern AEngine* CreateEngine();
	extern void DeleteEngine(AEngine* Engine);

	APRICOT_API extern AEngine* GEngine;

}