// Part of Apricot Engine. 2022-2022.
// Submodule: Layers

#pragma once

#include "Layer.h"

namespace Apricot {

	class APRICOT_API LayerStack
	{
	public:
		LayerStack() = default;

	public:
		void PushLayer(Layer* layer);

		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);

		void PopOverlay(Layer* overlay);

		void OnUpdate(Timestep ts);

		void OnEvent(AEvent* e);

	public:
		TVector<Layer*>& GetLayers() { return m_Layers; }
		const TVector<Layer*>& GetLayers() const { return m_Layers; }

		TVector<Layer*>& GetOverlays() { return m_Overlays; }
		const TVector<Layer*>& GetOverlays() const { return m_Overlays; }
	private:
		TVector<Layer*> m_Layers;
		TVector<Layer*> m_Overlays;
	};

}