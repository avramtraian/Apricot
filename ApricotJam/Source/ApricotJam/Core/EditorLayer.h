// Part of Apricot Engine. 2022-2022.
// Module: EditorCore

#pragma once

#include "Apricot/Core/Layer.h"

namespace Apricot {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() override;

		virtual uint64 GetLayerSize() const { return sizeof(EditorLayer); }

	public:
		virtual void OnAttach() override;

		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep Ts) override;

		virtual void OnEvent(AEvent* Ev) override;

	private:
		
	};

}