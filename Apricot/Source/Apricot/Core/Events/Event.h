#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/UUID.h"

namespace Apricot {

	enum class EventType : uint16
	{
		None = 0,

		KeyPressed,
		KeyReleased,

		MouseMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseWheelScrolled,

		WindowResized,
		WindowMoved,
		WindowClosed
	};

	class APRICOT_API Event
	{
	public:
		Event(EventType Type, UUID WindowUUID)
			: m_Type(Type), m_WindowUUID(WindowUUID) {}
		virtual ~Event() = default;

		EventType GetType() const { return m_Type; }
		UUID GetWindowUUID() const { return m_WindowUUID; }
		bool IsHandled() const { return m_Handled; }

	private:
		EventType m_Type = EventType::None;
		UUID m_WindowUUID = UUID::Invalid;
		bool m_Handled = false;

		friend class EventDispatcher;
	};

	class APRICOT_API EventDispatcher
	{
	public:
		template<typename T>
		using PFN_Dispatch = bool(*)(const T&);

	public:
		EventDispatcher(Event& E)
			: m_Event(E) {}

		template<typename T>
		void Dispatch(PFN_Dispatch<T> Function)
		{
			if (T::GetStaticEventType() == m_Event.GetType() && !m_Event.m_Handled)
			{
				m_Event.m_Handled = Function(*(const T*)(&m_Event));
			}
		}

	private:
		Event& m_Event;
	};

}