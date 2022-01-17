// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Event.h"
#include "Apricot/Input/InputCodes.h"

namespace Apricot {
	
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int32 mouseX, int32 mouseY)
			: Event(EventCategory::Mouse, EventType::MouseMoved), m_MouseX(mouseX), m_MouseY(mouseY) {}

		int32 GetMouseX() const { return m_MouseX; }
		int32 GetMouseY() const { return m_MouseY; }

		static EventType GetStaticType() { return EventType::MouseMoved; }

	private:
		int32 m_MouseX, m_MouseY;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(Button button)
			: Event(EventCategory::Mouse, EventType::MouseButtonPressed), m_Button(button) {}

		Button GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }

	private:
		Button m_Button = Button::None;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(Button button)
			: Event(EventCategory::Mouse, EventType::MouseButtonReleased), m_Button(button) {}

		Button GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }

	private:
		Button m_Button = Button::None;
	};

	class MouseWheelScrolledEvent : public Event
	{
	public:
		MouseWheelScrolledEvent(int32 offset)
			: Event(EventCategory::Mouse, EventType::MouseWheelScrolled), m_Offset(offset) {}

		int32 GetOffset() const { return m_Offset; }

		static EventType GetStaticType() { return EventType::MouseWheelScrolled; }

	private:
		int32 m_Offset;
	};

}