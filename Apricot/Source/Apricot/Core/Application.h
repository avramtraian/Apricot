#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API Application
	{
	protected:
		Application();
	public:
		virtual ~Application();

		int32 Run(const char* commandLine);

	private:
		
	};

	APRICOT_API extern Application* GApplication;

	extern Application* CreateApplication();

}