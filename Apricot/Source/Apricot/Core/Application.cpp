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

		for (auto it = m_LayerStack.Overlays().rbegin(); it != m_LayerStack.Overlays().rend(); it--)
		{
			(*it)->OnAttached();
		}

		for (auto it = m_LayerStack.Layers().rbegin(); it != m_LayerStack.Layers().rend(); it--)
		{
			(*it)->OnAttached();
		}

		Time lastTime;
		Platform::TimeGetSystemPerformanceTime(lastTime);

		while (m_Running)
		{
			Time now;
			Platform::TimeGetSystemPerformanceTime(now);
			Timestep ts = Timestep(lastTime, now);

			for (auto it = m_LayerStack.Overlays().rbegin(); it != m_LayerStack.Overlays().rend(); it--)
			{
				(*it)->OnUpdate(ts);
			}

			for (auto it = m_LayerStack.Layers().rbegin(); it != m_LayerStack.Layers().rend(); it--)
			{
				(*it)->OnUpdate(ts);
			}
		}

		for (auto& layer : m_LayerStack.Layers())
		{
			layer->OnDetached();
		}
		for (auto& overlay : m_LayerStack.Overlays())
		{
			overlay->OnDetached();
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