#pragma once

#include "Apricot/Core/Layer.h"

namespace Apricot {

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		virtual ~RuntimeLayer();

		virtual void OnAttached() override;
		virtual void OnDetached() override;

		virtual void OnUpdate(Timestep ts) override;

	private:
		
	};

}