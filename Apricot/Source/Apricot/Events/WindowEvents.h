// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Event.h"

namespace Apricot {

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
			: Event(EventCategory::Window, EventType::WindowClosed) {}

		static EventType GetStaticType() { return EventType::WindowClosed; }
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32 width, uint32 height)
			: Event(EventCategory::Window, EventType::WindowResized), m_Width(width), m_Height(height) {}

		static EventType GetStaticType() { return EventType::WindowResized; }

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }

	private:
		uint32 m_Width, m_Height;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(uint32 x, uint32 y)
			: Event(EventCategory::Window, EventType::WindowMoved), m_X(x), m_Y(y) {}

		static EventType GetStaticType() { return EventType::WindowMoved; }

		int32 GetX() const { return m_X; }
		int32 GetY() const { return m_Y; }

	private:
		int32 m_X, m_Y;
	};

}