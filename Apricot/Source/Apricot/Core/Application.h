#pragma once

#include "Base.h"

#include "LayerStack.h"
#include "Window.h"

namespace Apricot {
	
	struct ApplicationSpecification
	{
		ApplicationSpecification() = default;
		ApplicationSpecification(const ApplicationSpecification&) = default;
		ApplicationSpecification(ApplicationSpecification&&) noexcept = default;

		astl::string Name;
		astl::string WindowTitle;
		uint32 WindowWidth = 1, WindowHeight = 1;
		bool Fullscreen = false;
		bool Maximized = true;
		bool Minimized = false;
	};

	class APRICOT_API Application
	{
	protected:
		Application(const ApplicationSpecification& specification);
	public:
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		int32 Run(int32 Argc, char** Argv);

		bool OnEngineInit();
		void OnEngineDestroy();

		static void OnEvent(Event& E);

	public:
		static Application* Get();

		static void Quit(int32 QuitCode);

		static Window* AddWindow(const WindowSpecification& Specification);
		static void RemoveWindow(UUID WindowUUID);
		static Window* GetWindow(UUID WindowUUID);

	protected:
		LayerStack m_LayerStack;

	private:
		ApplicationSpecification m_Specification;
		bool m_Running = true;
		bool m_Minimized = false;

		Vector<astl::unique_ptr<Window>> m_Windows;
	};

	Application* CreateApplication();

}