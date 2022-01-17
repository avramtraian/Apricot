// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "EngineConfiguration.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API Engine
	{
	public:
		Engine();
		virtual ~Engine();
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

	public:
		int32 Run(const char8* commandArgs);

		void OnEvent(Event* e);

		void OnForceShutdown();

	private:
		virtual bool OnInitEngine(const char8* commandLine);
		virtual bool OnDestroyEngine();

		bool8 OnWindowClosed(const WindowClosedEvent* e);
		bool8 OnWindowResized(const WindowResizedEvent* e);
		bool8 OnWindowMoved(const WindowMovedEvent* e);

		bool8 OnKeyPressed(const KeyPressedEvent* e);
		bool8 OnKeyReleased(const KeyReleasedEvent* e);

		bool8 OnMouseMoved(const MouseMovedEvent* e);
		bool8 OnMouseButtonPressed(const MouseButtonPressedEvent* e);
		bool8 OnMouseButtonReleased(const MouseButtonReleasedEvent* e);
		bool8 OnMouseWheelScrolled(const MouseWheelScrolledEvent* e);

	public:
		 EngineConfig Config;

	private:
		 bool m_bIsRunning = true;
	};

	extern Engine* CreateEngine(uint64& engineSize);

	extern Engine* GEngine;

}