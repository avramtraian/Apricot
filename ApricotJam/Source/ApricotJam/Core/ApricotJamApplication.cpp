#include "ajpch.h"
#include "Apricot/Core/Application.h"
#include "Apricot/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Apricot {

	class ApricotJamApplication : public Application
	{
	public:
		ApricotJamApplication(const ApplicationSpecification& specification)
			: Application(specification)
		{
			m_LayerStack.PushLayer<EditorLayer>();
		}

		virtual ~ApricotJamApplication() override
		{
		}
	};

	extern Application* CreateApplication()
	{
		ApplicationSpecification specification;
		specification.Name = "ApricotJam-Editor";
		specification.WindowTitle = "ApricotJam -- Windows -- 64-bit -- Release -- Renderer: Vulkan";
		specification.WindowWidth = 0;
		specification.WindowHeight = 0;
		specification.Fullscreen = false;
		specification.Maximized = true;
		specification.Minimized = false;

		return anew ApricotJamApplication(specification);
	}

}