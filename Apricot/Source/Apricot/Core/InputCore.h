#pragma once

#include "Base.h"
#include "Events/Event.h"

namespace Apricot {

	enum class Key : uint16
	{
		None = 0,

		NumpadOne   = 1, NumpadTwo   = 2, NumpadThree = 3,
		NumpadFour  = 4, NumpadFive  = 5, NumpadSix   = 6,
		NumpadSeven = 7, NumpadEight = 8, NumpadNine  = 9,
		NumpadZero  = 10,

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

		Space       = NumpadZero + 1,
		Shift       = Space      + 1,
		Control     = Shift      + 1,
		Alt         = Control    + 1,
		Tab         = Alt        + 1,
		CapsLock    = Tab        + 1,
		Escape      = CapsLock   + 1,
		ArrowLeft   = Escape     + 1,
		ArrowRight  = ArrowLeft  + 1,
		ArrowUp     = ArrowRight + 1,
		ArrowDown   = ArrowUp    + 1,
		Backspace   = ArrowDown  + 1,
		Enter       = Backspace  + 1,

		F1  = Enter + 1, F2  = F1  + 1, F3  = F2  + 1,
		F4  = F3    + 1, F5  = F4  + 1, F6  = F5  + 1,
		F7  = F6    + 1, F8  = F7  + 1, F9  = F8  + 1,
		F10 = F9    + 1, F11 = F10 + 1, F12 = F11 + 1,

		Plus = F12 + 1, Comma = Plus + 1, Minus = Comma + 1, Period = Minus + 1,

		MaxEnumValue = 100
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