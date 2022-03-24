#include "aepch.h"
#include "Application.h"

#include <iostream>

namespace Apricot {

	APRICOT_API Application* GApplication = nullptr;

	Application::Application()
	{
	}
	
	Application::~Application()
	{
	}
	
	int32 Application::Run(const char* commandLine)
	{
		std::cout << "Hello, World!\n";
		return 0;
	}

}