#pragma once

#include "Base.h"
#include "Events/Event.h"

namespace Apricot {

	enum class Key : uint16
	{
		None = 0,

		NumpadOne,   NumpadTwo,   NumpadThree,
		NumpadFour,  NumpadFive,  NumpadSix,
		NumpadSeven, NumpadEight, NumpadNine,
		NumpadZero,

		One   = '1', Two   = '2', Three = '3',
		Four  = '4', Five  = '5', Six   = '6',
		Seven = '7', Eight = '8', Nine  = '9',
		Zero  = '0',

		A = 'A', B = 'B', C = 'C', D = 'D',
		E = 'E', F = 'F', G = 'G', H = 'H',
		I = 'I', J = 'J', K = 'K', L = 'L',
		M = 'M', N = 'N', O = 'O', P = 'P',
		Q = 'Q', R = 'R', S = 'S', T = 'T',
		U = 'U', V = 'V', W = 'W', X = 'X',
		Y = 'Y', Z = 'Z',

		Space = NumpadZero + 1,
		Shift,
		Control,
		Alt,
		Tab,
		CapsLock,
		NumLock,
		Escape,
		ArrowLeft,
		ArrowRight,
		ArrowUp,
		ArrowDown,
		Backspace,
		Enter,

		F1,  F2,  F3,
		F4,  F5,  F6,
		F7,  F8,  F9,
		F10, F11, F12,

		Plus,
		Minus,
		Period,
		Comma,
		Semicolon,
		Slash,
		Backtick,
		LeftSquareBracket,
		BackSlash,
		RightSquareBracket,
		Quote,

		NumpadAdd = 'Z' + 1,
		NumpadSubtract,
		NumpadMultiply,
		NumpadDivide,
		NumpadDecimal,

		MaxEnumValue
	};

	enum class Button : uint8
	{
		None = 0,
		Left = 1, Middle = 2, Right = 3,
		MaxEnumValue
	};

	/*
	* C++ Core
	* 
	* This class is designed to be use internally by the engine.
	* Any scripting code shouldn't interact with it.
	*/
	class APRICOT_API InputManager
	{
	public:
		static void OnTick();
		static void OnEvent(Event& E);
		static void OnApplicationMinimized();

		static bool IsKeyPressed(Key KeyCode);
		static bool IsMouseButtonPressed(Button ButtonCode);

		// static [int32, int32] GetMousePosition();
		static int32 GetMouseX();
		static int32 GetMouseY();
		static int32 GetMouseWheelOffset();

		// static [int32, int32] GetMouseOffsetLastTick();
		static int32 GetMouseXOffset();
		static int32 GetMouseYOffset();

		static bool WasMouseMoved();
		static bool WasMouseMovedOnX();
		static bool WasMouseMovedOnY();

		static bool WasKeyPressedThisTick(Key KeyCode);
		static bool WasKeyReleasedThisTick(Key KeyCode);

		static bool WasMouseButtonPressedThisTick(Button ButtonCode);
		static bool WasMouseButtonReleasedThisTick(Button ButtonCode);
	};

}