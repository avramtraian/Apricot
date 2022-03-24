#include "ajpch.h"
#include "Apricot/Core/Application.h"
#include "Apricot/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Apricot {

	class ApricotJamApplication : public Application
	{
	public:
		ApricotJamApplication()
		{
		}

		virtual ~ApricotJamApplication() override
		{
		}
	};

	extern Application* CreateApplication()
	{
		return new ApricotJamApplication();
	}

}