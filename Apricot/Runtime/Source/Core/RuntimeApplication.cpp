#include "Apricot/Core/Base.h"

#include <ASTL/string.h>

#include "Apricot/Core/Application.h"
#include "Apricot/Core/EntryPoint.h"

#include "RuntimeLayer.h"

namespace Apricot {

	class RuntimeApplication : public Application
	{
	public:
		RuntimeApplication(const ApplicationSpecification& specification)
			: Application(specification)
		{
		}

		virtual ~RuntimeApplication() override
		{
		}
	};

	extern Application* CreateApplication()
	{
		ApplicationSpecification specification;
		specification.Name = "ApricotRuntime";
		specification.WindowTitle = "ApricotRuntime -- Windows -- 64 bit";
		specification.WindowWidth = 0;
		specification.WindowHeight = 0;
		specification.Fullscreen = true;
		specification.Maximized = false;

		return new RuntimeApplication(specification);
	}

}