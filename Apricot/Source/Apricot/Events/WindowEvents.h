// Part of Apricot Engine. 2022-2022.
// Module: Events

#pragma once

#include "Event.h"

namespace Apricot {

	class AWindowClosedEvent : public AEvent
	{
	public:
		AWindowClosedEvent()
			: AEvent(EEventCategory::Window, EEventType::WindowClosed) {}

		static EEventType GetStaticType() { return EEventType::WindowClosed; }
	};

	class AWindowResizedEvent : public AEvent
	{
	public:
		AWindowResizedEvent(uint32 Width, uint32 Height)
			: AEvent(EEventCategory::Window, EEventType::WindowResized), m_Width(Width), m_Height(Height) {}

		static EEventType GetStaticType() { return EEventType::WindowResized; }

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }

	private:
		uint32 m_Width, m_Height;
	};

	class AWindowMovedEvent : public AEvent
	{
	public:
		AWindowMovedEvent(uint32 X, uint32 Y)
			: AEvent(EEventCategory::Window, EEventType::WindowMoved), m_X(X), m_Y(Y) {}

		static EEventType GetStaticType() { return EEventType::WindowMoved; }

		int32 GetX() const { return m_X; }
		int32 GetY() const { return m_Y; }

	private:
		int32 m_X, m_Y;
	};

}