#pragma once

#include "Application.h"

int main()
{
	Apricot::Application* application = Apricot::CreateApplication();
	int32 exitCode = application->Run("");
	delete application;
	return exitCode;
}