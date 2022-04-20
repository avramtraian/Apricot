#include "aepch.h"
#include "InputCore.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

namespace Apricot {

	struct InputManagerData
	{
		astl::array<bool, (uint16)Key::MaxEnumValue> KeysPressedStatus;
		astl::array<bool, (uint16)Key::MaxEnumValue> KeysPressedThisTick;
		astl::array<bool, (uint16)Key::MaxEnumValue> KeysReleasedThisTick;

		astl::array<bool, (uint8)Button::MaxEnumValue> MouseButtonsPressedStatus;
		astl::array<bool, (uint8)Button::MaxEnumValue> MouseButtonsPressedThisTick;
		astl::array<bool, (uint8)Button::MaxEnumValue> MouseButtonsReleasedThisTick;

		int32 MousePositionX = 0, MousePositionY = 0;
		int32 LastTickMousePositionX = 0, LastTickMousePositionY = 0;
		int32 MouseWheelOffset = 0;
	};
	static InputManagerData s_InputData;

	void InputManager::OnTick()
	{
		MemSet(s_InputData.KeysPressedThisTick.data(), false, s_InputData.KeysPressedThisTick.size() * sizeof(bool));
		MemSet(s_InputData.KeysReleasedThisTick.data(), false, s_InputData.KeysReleasedThisTick.size() * sizeof(bool));

		MemSet(s_InputData.MouseButtonsPressedThisTick.data(), false, s_InputData.MouseButtonsPressedThisTick.size() * sizeof(bool));
		MemSet(s_InputData.MouseButtonsReleasedThisTick.data(), false, s_InputData.MouseButtonsReleasedThisTick.size() * sizeof(bool));
		
		s_InputData.LastTickMousePositionX = s_InputData.MousePositionX;
		s_InputData.LastTickMousePositionY = s_InputData.MousePositionY;
		s_InputData.MouseWheelOffset = 0;
	}

	void InputManager::OnEvent(Event& E)
	{
		EventDispatcher Dispatcher(E);

		Dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& E) -> bool
		{
			s_InputData.KeysPressedStatus[(uint16)E.GetKeyCode()] = true;
			s_InputData.KeysPressedThisTick[(uint16)E.GetKeyCode()] = true;
			return false;
		});

		Dispatcher.Dispatch<KeyReleasedEvent>([](const KeyReleasedEvent& E) -> bool
		{
			s_InputData.KeysPressedStatus[(uint16)E.GetKeyCode()] = false;
			s_InputData.KeysReleasedThisTick[(uint16)E.GetKeyCode()] = true;
			return false;
		});

		Dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& E) -> bool
		{
			s_InputData.MouseButtonsPressedStatus[(uint16)E.GetButtonCode()] = true;
			s_InputData.MouseButtonsPressedThisTick[(uint16)E.GetButtonCode()] = true;
			return false;
		});

		Dispatcher.Dispatch<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& E) -> bool
		{
			s_InputData.MouseButtonsPressedStatus[(uint16)E.GetButtonCode()] = false;
			s_InputData.MouseButtonsReleasedThisTick[(uint16)E.GetButtonCode()] = true;
			return false;
		});

		Dispatcher.Dispatch<MouseMovedEvent>([](const MouseMovedEvent& E) -> bool
		{
			s_InputData.MousePositionX = E.GetMouseX();
			s_InputData.MousePositionY = E.GetMouseY();
			return false;
		});
	}

	void InputManager::OnApplicationMinimized()
	{
		MemSet(s_InputData.KeysPressedStatus.data(), false, s_InputData.KeysPressedStatus.size() * sizeof(bool));
		MemSet(s_InputData.KeysPressedThisTick.data(), false, s_InputData.KeysPressedThisTick.size() * sizeof(bool));
		MemSet(s_InputData.KeysReleasedThisTick.data(), false, s_InputData.KeysReleasedThisTick.size() * sizeof(bool));

		MemSet(s_InputData.MouseButtonsPressedStatus.data(), false, s_InputData.MouseButtonsPressedStatus.size() * sizeof(bool));
		MemSet(s_InputData.MouseButtonsPressedThisTick.data(), false, s_InputData.MouseButtonsPressedThisTick.size() * sizeof(bool));
		MemSet(s_InputData.MouseButtonsReleasedThisTick.data(), false, s_InputData.MouseButtonsReleasedThisTick.size() * sizeof(bool));

		s_InputData.MousePositionX = 0;
		s_InputData.MousePositionY = 0;
		s_InputData.LastTickMousePositionX = 0;
		s_InputData.LastTickMousePositionY = 0;
		s_InputData.MouseWheelOffset = 0;
	}

	bool InputManager::IsKeyPressed(Key KeyCode)
	{
		AE_CORE_ASSERT((uint16)KeyCode < (uint16)Key::MaxEnumValue);

		return s_InputData.KeysPressedStatus[(uint16)KeyCode];
	}

	bool InputManager::IsMouseButtonPressed(Button ButtonCode)
	{
		AE_CORE_ASSERT((uint8)ButtonCode < (uint8)Button::MaxEnumValue);

		return s_InputData.MouseButtonsPressedStatus[(uint8)ButtonCode];
	}

	int32 InputManager::GetMouseX()
	{
		return s_InputData.MousePositionX;
	}

	int32 InputManager::GetMouseY()
	{
		return s_InputData.MousePositionY;
	}

	int32 InputManager::GetMouseWheelOffset()
	{
		return s_InputData.MouseWheelOffset;
	}

	int32 InputManager::GetMouseXOffset()
	{
		return s_InputData.LastTickMousePositionX - s_InputData.MousePositionX;
	}

	int32 InputManager::GetMouseYOffset()
	{
		return s_InputData.LastTickMousePositionY - s_InputData.MousePositionY;
	}

	bool InputManager::WasMouseMoved()
	{
		return WasMouseMovedOnX() || WasMouseMovedOnY();
	}

	bool InputManager::WasMouseMovedOnX()
	{
		return s_InputData.LastTickMousePositionX != s_InputData.MousePositionX;
	}

	bool InputManager::WasMouseMovedOnY()
	{
		return s_InputData.LastTickMousePositionY != s_InputData.MousePositionY;
	}

	bool InputManager::WasKeyPressedThisTick(Key KeyCode)
	{
		AE_CORE_ASSERT((uint16)KeyCode < (uint16)Key::MaxEnumValue);

		return s_InputData.KeysPressedThisTick[(uint16)KeyCode];
	}

	bool InputManager::WasKeyReleasedThisTick(Key KeyCode)
	{
		AE_CORE_ASSERT((uint16)KeyCode < (uint16)Key::MaxEnumValue);

		return s_InputData.KeysReleasedThisTick[(uint16)KeyCode];
	}

	bool InputManager::WasMouseButtonPressedThisTick(Button ButtonCode)
	{
		AE_CORE_ASSERT((uint8)ButtonCode < (uint8)Button::MaxEnumValue);

		return s_InputData.MouseButtonsPressedThisTick[(uint8)ButtonCode];
	}

	bool InputManager::WasMouseButtonReleasedThisTick(Button ButtonCode)
	{
		AE_CORE_ASSERT((uint8)ButtonCode < (uint8)Button::MaxEnumValue);

		return s_InputData.MouseButtonsReleasedThisTick[(uint8)ButtonCode];
	}

}