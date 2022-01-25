// Part of Apricot Engine. 2022-2022.

#include "ajpch.h"

#include <Apricot/Core/EntryPoint.h>
#include <Apricot/Core/Engine.h>

namespace Apricot {
	
	class ApricotJamEngine : public AEngine
	{
	public:
		
	};

	AEngine* CreateEngine()
	{
		ApricotJamEngine* engine = (ApricotJamEngine*)Memory_Alloc(sizeof(ApricotJamEngine), AllocTag::Core);
		Memory_Placement<ApricotJamEngine>(engine);
		return engine;
	}

	void DeleteEngine(AEngine* engine)
	{
		((ApricotJamEngine*)engine)->~ApricotJamEngine();
		Memory_Free(engine, sizeof(ApricotJamEngine), AllocTag::Core);
	}

}