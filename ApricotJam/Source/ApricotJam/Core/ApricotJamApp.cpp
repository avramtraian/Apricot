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
		AApricotJamEngine* Engine = (AApricotJamEngine*)GMalloc->Alloc(sizeof(AApricotJamEngine));
		MemConstruct<AApricotJamEngine>(Engine);
		return Engine;
	}

	void DeleteEngine(AEngine* Engine)
	{
		((AApricotJamEngine*)Engine)->~AApricotJamEngine();
		GMalloc->Free(Engine, sizeof(AApricotJamEngine));
	}

}