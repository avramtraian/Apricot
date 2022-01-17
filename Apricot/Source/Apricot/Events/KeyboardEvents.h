// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Event.h"
#include "Apricot/Input/InputCodes.h"

namespace Apricot {

	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(Key key)
			: Event(EventCategory::Keyboard, EventType::KeyPressed), m_Key(key) {}

		Key GetKey() const { return m_Key; }

		static EventType GetStaticType() { return EventType::KeyPressed; }

	private:
		Key m_Key = Key::None;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(Key key)
			: Event(EventCategory::Keyboard, EventType::KeyReleased), m_Key(key) {}

		Key GetKey() const { return m_Key; }

		static EventType GetStaticType() { return EventType::KeyReleased; }

	private:
		Key m_Key = Key::None;
	};

}