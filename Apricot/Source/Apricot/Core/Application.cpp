#include "aepch.h"
#include "Application.h"

namespace Apricot {

	APRICOT_API Application* GApplication = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
	}
	
	Application::~Application()
	{
	}
	
	int32 Application::Run(const char* commandLine)
	{
		return 0;
	}

}