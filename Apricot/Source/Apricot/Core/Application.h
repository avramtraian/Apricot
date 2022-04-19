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

		int32 Run(int32 argc, char** argv);

		bool OnEngineInit();
		void OnEngineDestroy();

	public:
		static Application* Get();

		static void Quit(int32 QuitCode);

		static UUID AddWindow(const WindowSpecification& Specification);
		static void RemoveWindow(UUID WindowUUID);
		static Window* GetWindow(UUID WindowUUID);

	protected:
		LayerStack m_LayerStack;

	private:
		ApplicationSpecification m_Specification;
		bool m_Running = true;
		bool m_Minimized = false;

		astl::vector<astl::unique_ptr<Window>> m_Windows;
	};

	extern Application* CreateApplication();

}