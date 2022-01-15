// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "EngineConfiguration.h"

namespace Apricot {

	class APRICOT_API Engine
	{
	public:
		Engine();
		virtual ~Engine();
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

	public:
		int32 Run(const char8* commandArgs);

		void OnForceShutdown();

	private:
		virtual bool OnPreInitEngine(const char8* commandLine);
		virtual bool OnInitEngine();
		virtual bool OnDestroyEngine();

	public:
		 EngineConfig Config;

	private:
		 bool m_bIsRunning = true;
	};

	extern Engine* CreateEngine(uint64& engineSize);

	extern Engine* GEngine;

}