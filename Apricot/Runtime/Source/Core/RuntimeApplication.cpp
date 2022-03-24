#include "Apricot/Core/Application.h"
#include "Apricot/Core/EntryPoint.h"

#include "RuntimeLayer.h"

namespace Apricot {

	class RuntimeApplication : public Application
	{
	public:
		RuntimeApplication()
		{
		}

		virtual ~RuntimeApplication() override
		{
		}
	};

	extern Application* CreateApplication()
	{
		return new RuntimeApplication();
	}

}