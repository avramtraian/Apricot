#pragma once

#include "Apricot/Core/Layer.h"

namespace Apricot {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttached() override;
		virtual void OnDetached() override;

		virtual void OnUpdate(Timestep ts) override;

	private:
		
	};

}