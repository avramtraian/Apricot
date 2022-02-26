// Part of Apricot Engine. 2022-2022.
// Module: Events

#pragma once

#include "Event.h"
#include "Apricot/Input/InputCodes.h"

namespace Apricot {
	
	class AMouseMovedEvent : public AEvent
	{
	public:
		AMouseMovedEvent(int32 MouseX, int32 MouseY)
			: AEvent(EEventCategory::Mouse, EEventType::MouseMoved), m_MouseX(MouseX), m_MouseY(MouseY) {}

		int32 GetMouseX() const { return m_MouseX; }
		int32 GetMouseY() const { return m_MouseY; }

		static EEventType GetStaticType() { return EEventType::MouseMoved; }

	private:
		int32 m_MouseX, m_MouseY;
	};

	class AMouseButtonPressedEvent : public AEvent
	{
	public:
		AMouseButtonPressedEvent(EButton Button)
			: AEvent(EEventCategory::Mouse, EEventType::MouseButtonPressed), m_Button(Button) {}

		EButton GetButton() const { return m_Button; }

		static EEventType GetStaticType() { return EEventType::MouseButtonPressed; }

	private:
		EButton m_Button = EButton::None;
	};

	class AMouseButtonReleasedEvent : public AEvent
	{
	public:
		AMouseButtonReleasedEvent(EButton Button)
			: AEvent(EEventCategory::Mouse, EEventType::MouseButtonReleased), m_Button(Button) {}

		EButton GetButton() const { return m_Button; }

		static EEventType GetStaticType() { return EEventType::MouseButtonReleased; }

	private:
		EButton m_Button = EButton::None;
	};

	class AMouseWheelScrolledEvent : public AEvent
	{
	public:
		AMouseWheelScrolledEvent(int32 Offset)
			: AEvent(EEventCategory::Mouse, EEventType::MouseWheelScrolled), m_Offset(Offset) {}

		int32 GetOffset() const { return m_Offset; }

		static EEventType GetStaticType() { return EEventType::MouseWheelScrolled; }

	private:
		int32 m_Offset;
	};

}