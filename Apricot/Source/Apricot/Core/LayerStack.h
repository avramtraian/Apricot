#pragma once

#include "Layer.h"

namespace Apricot {

	class APRICOT_API LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		template<typename T, typename... Args>
		void PushLayer(Args&&... args)
		{
			PushLayer(anew T(astl::forward<Args>(args)...));
		}

		template<typename T, typename... Args>
		void PushOverlay(Args&&... args)
		{
			PushOverlay(anew T(astl::forward<Args>(args)...));
		}

	public:
		Vector<Layer*>& Layers() { return m_Layers; }
		Vector<Layer*>& Overlays() { return m_Overlays; }

		const Vector<Layer*>& Layers() const { return m_Layers; }
		const Vector<Layer*>& Overlays() const { return m_Overlays; }


	private:
		Vector<Layer*> m_Layers;
		Vector<Layer*> m_Overlays;
	};

}