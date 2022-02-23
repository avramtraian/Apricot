// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Event.h"
#include "Apricot/Input/InputCodes.h"

namespace Apricot {

	class AKeyPressedEvent : public AEvent
	{
	public:
		AKeyPressedEvent(EKey Key)
			: AEvent(EEventCategory::Keyboard, EEventType::KeyPressed), m_Key(Key) {}

		EKey GetKey() const { return m_Key; }

		static EEventType GetStaticType() { return EEventType::KeyPressed; }

	private:
		EKey m_Key = EKey::None;
	};

	class AKeyReleasedEvent : public AEvent
	{
	public:
		AKeyReleasedEvent(EKey Key)
			: AEvent(EEventCategory::Keyboard, EEventType::KeyReleased), m_Key(Key) {}

		EKey GetKey() const { return m_Key; }

		static EEventType GetStaticType() { return EEventType::KeyReleased; }

	private:
		EKey m_Key = EKey::None;
	};

}