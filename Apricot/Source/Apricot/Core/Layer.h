// Part of Apricot Engine. 2022-2022.
// Submodule: Layers

#pragma once

#include "Base.h"
#include "Time.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API Layer
	{
	public:
		Layer(const char* DebugName)
			: m_DebugName(DebugName)
		{
		}

		virtual ~Layer() = default;

	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep Ts) {}
		virtual void OnEvent(AEvent* Ev) {}

	public:
		virtual uint64 GetLayerSize() const = 0;

		const String& GetLayerDebugName() const { return m_DebugName; }

	private:
		String m_DebugName;
	};

}