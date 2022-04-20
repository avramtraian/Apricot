#include "aepch.h"
#include "Application.h"

#include "Platform.h"
#include "InputCore.h"

namespace Apricot {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Application = this;
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
			for (auto& Wnd : m_Windows)
			{
				Wnd->UpdateWindow();
			}

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

			InputManager::OnTick();

			for (uint64 Index = 0; Index < m_Windows.size(); Index++)
			{
				if (m_Windows[Index]->ShouldClose())
				{
					if (m_Windows[Index]->IsPrimary())
					{
						m_Running = false;
					}
					m_Windows.erase(Index);
				}
			}
			if (m_Windows.size() == 0)
			{
				m_Running = false;
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

		m_Windows.clear();

		AE_CORE_INFO_TAG("Engine", "Shutting down!");
		OnEngineDestroy();

		return 0;
	}

	bool Application::OnEngineInit()
	{
		Logger::Init();

		WindowSpecification MainWindowSpecification;
		MainWindowSpecification.Width = m_Specification.WindowWidth;
		MainWindowSpecification.Height = m_Specification.WindowHeight;
		MainWindowSpecification.Maximized = m_Specification.Maximized;
		MainWindowSpecification.Minimized = m_Specification.Minimized;
		MainWindowSpecification.Fullscreen = m_Specification.Fullscreen;
		MainWindowSpecification.Title = m_Specification.WindowTitle;
		MainWindowSpecification.EventCallback = Application::OnEvent;
		Window* MainWindow = s_Application->AddWindow(MainWindowSpecification);
		MainWindow->SetPrimary(true);

		return true;
	}

	void Application::OnEngineDestroy()
	{
		Logger::Destroy();
	}

	void Application::OnEvent(Event& E)
	{
		InputManager::OnEvent(E);
	}

	Application* Application::Get()
	{
		return s_Application;
	}

	void Application::Quit(int32 QuitCode)
	{
		s_Application->m_Running = false;
	}

	Window* Application::AddWindow(const WindowSpecification& Specification)
	{
		auto& NewWindow = s_Application->m_Windows.emplace_back(Window::Create(Specification));
		return NewWindow.get();
	}

	void Application::RemoveWindow(UUID WindowUUID)
	{
		for (uint64 Index = 0; Index < s_Application->m_Windows.size(); Index++)
		{
			if (s_Application->m_Windows[Index]->GetUUID() == WindowUUID)
			{
				s_Application->m_Windows.erase(Index);
				break;
			}
		}
	}

	Window* Application::GetWindow(UUID WindowUUID)
	{
		for (auto& Win : s_Application->m_Windows)
		{
			if (Win->GetUUID() == WindowUUID)
			{
				return Win.get();
			}
		}
		return nullptr;
	}

}