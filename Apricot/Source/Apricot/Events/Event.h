// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Log.h"

#define AE_EVENT_CASE(Type) \
	case EventType::Type: \
	{ \
		m_Event->m_bIsHandled = m_Map->On##Type((const Type##Event*)m_Event);\
		break;\
	}

namespace Apricot {
	
	enum class EventCategory : uint8
	{
		None = 0,
		Unknown = 1,

		Application = 1 << 1,
		Window      = 1 << 2,
		Keyboard    = 1 << 3,
		Mouse       = 1 << 4
	};

	enum class EventType : uint16
	{
		None = 0,
		Unknown = 1,

		WindowClosed        = (uint16)EventCategory::Window | (1 << 5),
		WindowResized       = (uint16)EventCategory::Window | (1 << 6),
		WindowMoved         = (uint16)EventCategory::Window | (1 << 7),
					        
		KeyPressed          = (uint16)EventCategory::Keyboard | (1 << 5),
		KeyReleased         = (uint16)EventCategory::Keyboard | (1 << 6),

		MouseMoved          = (uint16)EventCategory::Mouse | (1 << 5),
		MouseButtonPressed  = (uint16)EventCategory::Mouse | (1 << 6),
		MouseButtonReleased = (uint16)EventCategory::Mouse | (1 << 7),
		MouseWheelScrolled  = (uint16)EventCategory::Mouse | (1 << 8)
	};

	class Event
	{
	public:
		Event(EventCategory category, EventType type)
			: m_Category(category), m_Type(type) {}

		EventCategory GetCategory() const { return m_Category; }
		EventType GetType() const { return m_Type; }

		static EventType GetStaticType() { return EventType::None; }

	private:
		EventCategory m_Category = EventCategory::None;
		EventType m_Type = EventType::None;
		bool8 m_bIsHandled = false;

		friend class EventDispatcher;
	};

	// Forward declare all event classes.
	class WindowClosedEvent;
	class WindowResizedEvent;
	class WindowMovedEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseMovedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseWheelScrolledEvent;

	struct EventDispatchMap
	{
		template<typename T>
		using PFN_Dispatch = bool8(*)(const T* e);

		PFN_Dispatch<Event> OnUnknown                                = [](const Event* e) -> bool8 { return false; };
		PFN_Dispatch<WindowClosedEvent> OnWindowClosed               = [](const WindowClosedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<WindowResizedEvent> OnWindowResized             = [](const WindowResizedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<WindowMovedEvent> OnWindowMoved                 = [](const WindowMovedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<KeyPressedEvent> OnKeyPressed                   = [](const KeyPressedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<KeyReleasedEvent> OnKeyReleased                 = [](const KeyReleasedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<MouseMovedEvent> OnMouseMoved                   = [](const MouseMovedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<MouseButtonPressedEvent> OnMouseButtonPressed   = [](const MouseButtonPressedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<MouseButtonReleasedEvent> OnMouseButtonReleased = [](const MouseButtonReleasedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<MouseWheelScrolledEvent> OnMouseWheelScrolled   = [](const MouseWheelScrolledEvent* e) -> bool8 { return false; };
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event* e, EventDispatchMap* map)
			: m_Event(e), m_Map(map) {}

		void Dispatch() const
		{
			if (m_Event->m_bIsHandled)
			{
				return;
			}

			switch (m_Event->GetType())
			{
				AE_EVENT_CASE(WindowClosed)
				AE_EVENT_CASE(WindowResized)
				AE_EVENT_CASE(WindowMoved)
				AE_EVENT_CASE(KeyPressed)
				AE_EVENT_CASE(KeyReleased)
				AE_EVENT_CASE(MouseMoved)
				AE_EVENT_CASE(MouseButtonPressed)
				AE_EVENT_CASE(MouseButtonReleased)
				AE_EVENT_CASE(MouseWheelScrolled)

				case EventType::Unknown:
				{
					m_Event->m_bIsHandled = m_Map->OnUnknown(m_Event);
					break;
				}
				case EventType::None:
				{
					AE_CORE_ERROR("Invalid '{}' EventType!", (uint16)m_Event->GetType());
					break;
				}
				default:
				{
					AE_CORE_ERROR("Corrupt '{}' EventType!", (uint16)m_Event->GetType());
					break;
				}
			}
		}

		template<typename T, typename Func>
		void Dispatch(Func callback) const
		{
			if (!m_Event->m_bIsHandled && m_Event->GetType() == T::GetStaticType())
			{
				m_Event->m_bIsHandled = callback((const T*)m_Event);
			}
		}

	private:
		Event* m_Event = nullptr;
		EventDispatchMap* m_Map = nullptr;
	};

}