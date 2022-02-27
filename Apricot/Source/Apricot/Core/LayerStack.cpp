// Part of Apricot Engine. 2022-2022.
// Submodule: Layers

#include "aepch.h"
#include "LayerStack.h"

namespace Apricot {

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.PushBack(layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.PushBack(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		for (TVector<Layer*>::TIterator it = m_Layers.begin(); it != m_Layers.end(); it++)
		{
			if ((*it) == layer)
			{
				m_Layers.Erase(it);
				break;
			}
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		for (TVector<Layer*>::TIterator it = m_Overlays.begin(); it != m_Overlays.end(); it++)
		{
			if ((*it) == overlay)
			{
				m_Overlays.Erase(it);
				break;
			}
		}
	}

	void LayerStack::OnUpdate(Timestep ts)
	{
		for (auto layer : m_Layers)
		{
			layer->OnUpdate(ts);
		}
		for (auto overlay : m_Overlays)
		{
			overlay->OnUpdate(ts);
		}
	}

	void LayerStack::OnEvent(AEvent* e)
	{
		for (TVector<Layer*>::TReverseIterator it = m_Overlays.rbegin(); it != m_Overlays.rend(); it--)
		{
			(*it)->OnEvent(e);
		}
		for (TVector<Layer*>::TReverseIterator it = m_Layers.rbegin(); it != m_Layers.rend(); it--)
		{
			(*it)->OnEvent(e);
		}
	}

}