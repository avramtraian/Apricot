#include "ajpch.h"
#include <Apricot/Core/EntryPoint.h>

namespace Apricot {

	class CherryJamEngine : public Engine
	{
	public:
		virtual ~CherryJamEngine() override = default;
	};

	Engine* CreateEngine(uint64& engineSize)
	{
		AE_CORE_INFO("Instantiating the Engine!");
		engineSize = sizeof(CherryJamEngine);
		return Memory::New<CherryJamEngine>(Memory::AllocTag::CoreSystems);
	}

}