#include "aepch.h"
#include "Engine.h"

namespace Apricot {

	Engine* Engine::s_Engine = nullptr;

	Engine::Engine()
	{
		
	}

	Engine::~Engine()
	{

	}

	int32 Engine::Run(const char* commandArgs)
	{
		s_Engine = this;

		OnInitEngine();



		OnDestroyEngine();

		return 0;
	}

	bool Engine::OnInitEngine()
	{
		return true;
	}

	bool Engine::OnDestroyEngine()
	{
		return true;
	}

}