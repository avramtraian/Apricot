#include "ajpch.h"
#include <Apricot/Core/EntryPoint.h>

namespace Apricot {

	class CherryJamEngine : public Engine
	{
	public:
		virtual ~CherryJamEngine() override = default;
	};

	Engine* CreateEngine()
	{
		return new CherryJamEngine();
	}

}