#pragma once

#include "Event.h"
#include "Apricot/Core/InputCore.h"

namespace Apricot {

	class APRICOT_API KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(Key KeyCode, uint32 RepeatCount, UUID WindowUUID)
			: Event(EventType::KeyPressed, WindowUUID)
			, m_KeyCode(KeyCode), m_RepeatCount(RepeatCount) {}

		static EventType GetStaticEventType() { return EventType::KeyPressed; }

		Key GetKeyCode() const { return m_KeyCode; }
		uint32 GetRepeatCount() const { return m_RepeatCount; }

	private:
		Key m_KeyCode;
		uint32 m_RepeatCount;
	};

	class APRICOT_API KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(Key KeyCode, UUID WindowUUID)
			: Event(EventType::KeyReleased, WindowUUID)
			, m_KeyCode(KeyCode) {}

		static EventType GetStaticEventType() { return EventType::KeyReleased; }

		Key GetKeyCode() const { return m_KeyCode; }

	private:
		Key m_KeyCode;
	};

}