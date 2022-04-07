#pragma once

#include "Base.h"

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

	private:
		ApplicationSpecification m_Specification;
		bool m_Running = true;
		bool m_Minimized = false;
	};

	APRICOT_API extern Application* GApplication;

	extern Application* CreateApplication();

}