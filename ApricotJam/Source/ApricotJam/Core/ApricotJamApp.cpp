// Part of Apricot Engine. 2022-2022.

#include "ajpch.h"

#include <Apricot/Core/EntryPoint.h>
#include <Apricot/Core/Engine.h>

namespace Apricot {
	
	class AApricotJamEngine : public AEngine
	{
	public:
		
	};

	AEngine* CreateEngine()
	{
		AApricotJamEngine* Engine = (AApricotJamEngine*)Memory_Alloc(sizeof(AApricotJamEngine), EAllocTag::Core);
		Memory_Placement<AApricotJamEngine>(Engine);
		return Engine;
	}

	void DeleteEngine(AEngine* Engine)
	{
		((AApricotJamEngine*)Engine)->~AApricotJamEngine();
		Memory_Free(Engine, sizeof(AApricotJamEngine), EAllocTag::Core);
	}

}