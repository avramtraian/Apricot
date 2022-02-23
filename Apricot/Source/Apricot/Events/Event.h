// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Log.h"

#define AE_EVENT_CASE(Type) \
	case EEventType::Type: \
	{ \
		m_Event->m_bIsHandled = m_Map->On##Type((const A##Type##Event*)m_Event);\
		break;\
	}

namespace Apricot {
	
	enum class EEventCategory : uint8
	{
		None = 0,
		Unknown = 1,

		Application = 1 << 1,
		Window      = 1 << 2,
		Keyboard    = 1 << 3,
		Mouse       = 1 << 4
	};

	enum class EEventType : uint16
	{
		None = 0,
		Unknown = 1,

		WindowClosed        = (uint16)EEventCategory::Window | (1 << 5),
		WindowResized       = (uint16)EEventCategory::Window | (1 << 6),
		WindowMoved         = (uint16)EEventCategory::Window | (1 << 7),
					        
		KeyPressed          = (uint16)EEventCategory::Keyboard | (1 << 5),
		KeyReleased         = (uint16)EEventCategory::Keyboard | (1 << 6),

		MouseMoved          = (uint16)EEventCategory::Mouse | (1 << 5),
		MouseButtonPressed  = (uint16)EEventCategory::Mouse | (1 << 6),
		MouseButtonReleased = (uint16)EEventCategory::Mouse | (1 << 7),
		MouseWheelScrolled  = (uint16)EEventCategory::Mouse | (1 << 8)
	};

	class AEvent
	{
	public:
		AEvent(EEventCategory Category, EEventType Type)
			: m_Category(Category), m_Type(Type) {}

		EEventCategory GetCategory() const { return m_Category; }
		EEventType GetType() const { return m_Type; }

		static EEventType GetStaticType() { return EEventType::None; }

	private:
		EEventCategory m_Category = EEventCategory::None;
		EEventType m_Type = EEventType::None;
		bool8 m_bIsHandled = false;

		friend class AEventDispatcher;
	};

	// Forward declare all event classes.
	class AWindowClosedEvent;
	class AWindowResizedEvent;
	class AWindowMovedEvent;
	class AKeyPressedEvent;
	class AKeyReleasedEvent;
	class AMouseMovedEvent;
	class AMouseButtonPressedEvent;
	class AMouseButtonReleasedEvent;
	class AMouseWheelScrolledEvent;

	struct AEventDispatchMap
	{
		template<typename T>
		using PFN_Dispatch = bool8(*)(const T* Ev);

		PFN_Dispatch<AEvent> OnUnknown                                = [](const AEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AWindowClosedEvent> OnWindowClosed               = [](const AWindowClosedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AWindowResizedEvent> OnWindowResized             = [](const AWindowResizedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AWindowMovedEvent> OnWindowMoved                 = [](const AWindowMovedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AKeyPressedEvent> OnKeyPressed                   = [](const AKeyPressedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AKeyReleasedEvent> OnKeyReleased                 = [](const AKeyReleasedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AMouseMovedEvent> OnMouseMoved                   = [](const AMouseMovedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AMouseButtonPressedEvent> OnMouseButtonPressed   = [](const AMouseButtonPressedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AMouseButtonReleasedEvent> OnMouseButtonReleased = [](const AMouseButtonReleasedEvent* e) -> bool8 { return false; };
		PFN_Dispatch<AMouseWheelScrolledEvent> OnMouseWheelScrolled   = [](const AMouseWheelScrolledEvent* e) -> bool8 { return false; };
	};

	class AEventDispatcher
	{
	public:
		AEventDispatcher(AEvent* Event, AEventDispatchMap* Map)
			: m_Event(Event), m_Map(Map) {}

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

				case EEventType::Unknown:
				{
					m_Event->m_bIsHandled = m_Map->OnUnknown(m_Event);
					break;
				}
				case EEventType::None:
				{
					AE_CORE_ERROR(TEXT("Invalid '{}' EventType!"), (uint16)m_Event->GetType());
					break;
				}
				default:
				{
					AE_CORE_ERROR(TEXT("Corrupt '{}' EventType!"), (uint16)m_Event->GetType());
					break;
				}
			}
		}

		template<typename T, typename Func>
		void Dispatch(Func Callback) const
		{
			if (!m_Event->m_bIsHandled && m_Event->GetType() == T::GetStaticType())
			{
				m_Event->m_bIsHandled = Callback((const T*)m_Event);
			}
		}

	private:
		AEvent* m_Event = nullptr;
		AEventDispatchMap* m_Map = nullptr;
	};

}