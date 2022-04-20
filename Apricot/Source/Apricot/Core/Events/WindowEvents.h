#pragma once

#include "Event.h"

namespace Apricot {

	class APRICOT_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent(UUID WindowUUID)
			: Event(EventType::WindowClosed, WindowUUID) {}

		static EventType GetStaticEventType() { return EventType::WindowClosed; }
	};

	class APRICOT_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int32 X, int32 Y, UUID WindowUUID)
			: Event(EventType::WindowMoved, WindowUUID)
			, m_X(X), m_Y(Y) {}

		int32 GetX() const { return m_X; }
		int32 GetY() const { return m_Y; }

	private:
		int32 m_X, m_Y;
	};

	class APRICOT_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32 Width, uint32 Height, UUID WindowUUID)
			: Event(EventType::WindowResized, WindowUUID)
			, m_Width(Width), m_Height(Height) {}

		static EventType GetStaticEventType() { return EventType::WindowResized; }

		int32 GetWidth() const { return m_Width; }
		int32 GetHeight() const { return m_Height; }

	private:
		uint32 m_Width, m_Height;
	};

}