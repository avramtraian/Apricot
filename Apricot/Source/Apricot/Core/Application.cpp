#include "aepch.h"
#include "Application.h"

#include "Platform.h"

namespace Apricot {

	APRICOT_API Application* GApplication = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
	}
	
	Application::~Application()
	{
	}
	
	int32 Application::Run(int32 argc, char** argv)
	{
		if (!OnEngineInit())
		{
			return -1;
		}
		AE_CORE_INFO_TAG("Engine", "Initializing done!");

		while (m_Running)
		{
			Time time;
			Platform::TimeGetSystemPerformanceTime(time);
			float s = time.Seconds();
			AE_CORE_INFO("hmmm");
		}

		AE_CORE_INFO_TAG("Engine", "Shutting down!");
		OnEngineDestroy();

		return 0;
	}

	bool Application::OnEngineInit()
	{
		Logger::Init();
		return true;
	}

	void Application::OnEngineDestroy()
	{
		Logger::Destroy();
	}

}