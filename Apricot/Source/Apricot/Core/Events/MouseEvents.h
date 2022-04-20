#pragma once

#include "Event.h"
#include "Apricot/Core/InputCore.h"

namespace Apricot {

	class APRICOT_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int32 X, int32 Y, UUID WindowUUID)
			: Event(EventType::MouseMoved, WindowUUID)
			, m_X(X), m_Y(Y) {}

		static EventType GetStaticEventType() { return EventType::MouseMoved; }

		int32 GetMouseX() const { return m_X; }
		int32 GetMouseY() const { return m_Y; }

	private:
		int32 m_X, m_Y;
	};

	class APRICOT_API MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(Button ButtonCode, UUID WindowUUID)
			: Event(EventType::MouseButtonPressed, WindowUUID)
			, m_ButtonCode(ButtonCode) {}

		static EventType GetStaticEventType() { return EventType::MouseButtonPressed; }

		Button GetButtonCode() const { return m_ButtonCode; }

	private:
		Button m_ButtonCode;
	};

	class APRICOT_API MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(Button ButtonCode, UUID WindowUUID)
			: Event(EventType::MouseButtonReleased, WindowUUID)
			, m_ButtonCode(ButtonCode) {}

		static EventType GetStaticEventType() { return EventType::MouseButtonReleased; }

		Button GetButtonCode() const { return m_ButtonCode; }

	private:
		Button m_ButtonCode;
	};

	class APRICOT_API MouseWheelScrolledEvent : public Event
	{
	public:
		MouseWheelScrolledEvent(int32 Offset, UUID WindowUUID)
			: Event(EventType::MouseWheelScrolled, WindowUUID)
			, m_Offset(Offset) {}

		static EventType GetStaticEventType() { return EventType::MouseWheelScrolled; }

		int32 GetOffset() const { return m_Offset; }

	private:
		int32 m_Offset;
	};

}