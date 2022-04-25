#include "aepch.h"
#include "LayerStack.h"

namespace Apricot {

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.Add(layer);
		layer->OnCreate();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Overlays.Add(overlay);
		overlay->OnCreate();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		for (uint64 index = 0; index < m_Layers.Size(); index++)
		{
			if (m_Layers[index] == layer)
			{
				layer->OnDestroy();
				adelete m_Layers[index];
				m_Layers.RemoveAt(index);
				break;
			}
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		for (uint64 index = 0; index < m_Overlays.Size(); index++)
		{
			if (m_Overlays[index] == overlay)
			{
				overlay->OnDestroy();
				adelete m_Overlays[index];
				m_Overlays.RemoveAt(index);
				break;
			}
		}
	}

}