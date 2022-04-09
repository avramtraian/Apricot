#pragma once

#include "Base.h"
#include "Time.h"

namespace Apricot {

	class APRICOT_API Layer
	{
	public:
		Layer(const char* layerName)
			: m_LayerName(layerName) {}
		virtual ~Layer() = default;

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}

		virtual void OnAttached() {}
		virtual void OnDetached() {}

		virtual void OnUpdate(Timestep ts) {}

	protected:
		const char* m_LayerName;
	};

}