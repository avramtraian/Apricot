// Part of Apricot Engine. 2022-2022.
// Submodule: Layers

#pragma once

#include "Base.h"
#include "Types.h"
#include "Timestep.h"

#include "Apricot/Events/Event.h"

namespace Apricot {

	class APRICOT_API Layer
	{
	public:
		Layer(const TChar* DebugName)
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

		const AString& GetLayerDebugName() const { return m_DebugName; }

	private:
		AString m_DebugName;
	};

}