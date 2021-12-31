#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API Engine
	{
	public:
		Engine();
		virtual ~Engine();
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

	public:
		int32 Run(const char* commandArgs);

		void OnForceShutdown();

	private:
		virtual bool OnInitEngine();
		virtual bool OnDestroyEngine();

	private:
		 bool m_bIsRunning = true;
	};

	extern Engine* CreateEngine(uint64& engineSize);

	extern Engine* GEngine;

}