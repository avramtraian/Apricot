#include "aepch.h"

#include "Apricot/Core/Window.h"
#include "Apricot/Core/Platform.h"
#include "Apricot/Core/Application.h"

#include "WindowsHeaders.h"

#include "Apricot/Core/Events/Event.h"
#include "Apricot/Core/Events/WindowEvents.h"
#include "Apricot/Core/Events/KeyEvents.h"
#include "Apricot/Core/Events/MouseEvents.h"

namespace Apricot {

	static const wchar_t* s_WindowClassName = L"Apricot Window Class";

	static LRESULT WindowEventProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	static Window* s_CurrentUpdatingWindow = nullptr;

	namespace Utils {

		static HMONITOR GetPrimaryMonitor()
		{
			HMONITOR Primary = NULL;

			EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwDat) -> BOOL
			{
				MONITORINFOEX Monitor = { 0 };
				GetMonitorInfo(hMonitor, &Monitor);

				if (lprcMonitor->left == 0 && lprcMonitor->top == 0)
				{
					*((HMONITOR*)dwDat) = hMonitor;
				}

				return TRUE;
			},
			(LPARAM)& Primary);

			return Primary;
		}

		struct MonitorInfo
		{
			uint32 Width = 0, Height = 0;
			int32 X = 0, Y = 0;
			bool Primary = false;
		};

		static void GetMonitorInfo(HMONITOR Monitor, MonitorInfo& Info)
		{
			struct UserDataStruct
			{
				HMONITOR* Monitor;
				MonitorInfo* Info;
			};

			UserDataStruct Data = { 0 };
			Data.Info = &Info;
			Data.Monitor = &Monitor;

			EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwDat) -> BOOL
				{
					MONITORINFOEX Monitor = { 0 };
					GetMonitorInfo(hMonitor, &Monitor);
					UserDataStruct& UserData = *((UserDataStruct*)dwDat);

					if (*UserData.Monitor == hMonitor)
					{
						UserData.Info->Width = lprcMonitor->right - lprcMonitor->left;
						UserData.Info->Height = lprcMonitor->bottom - lprcMonitor->top;
						UserData.Info->X = lprcMonitor->left;
						UserData.Info->Y = lprcMonitor->top;
						UserData.Info->Primary = (UserData.Info->X == 0 && UserData.Info->Y == 0);

						return FALSE;
					}

					return TRUE;
				},
				(LPARAM)&Data);
		}

		static void RegisterWindowClass()
		{
			WNDCLASSEX WndClass = {};
			WndClass.cbSize = sizeof(WNDCLASSEX);
			WndClass.style = 0;
			WndClass.lpfnWndProc = WindowEventProcedure;
			WndClass.cbClsExtra = 0;
			WndClass.cbWndExtra = 0;
			WndClass.hInstance = (HINSTANCE)Platform::WindowsGetInstance();
			WndClass.hIcon = NULL;
			WndClass.hIconSm = NULL;
			WndClass.hCursor = NULL;
			WndClass.hbrBackground = NULL;
			WndClass.lpszMenuName = NULL;
			WndClass.lpszClassName = s_WindowClassName;

			RegisterClassEx(&WndClass);
		}

	}

	Window* Window::Create(const WindowSpecification& specification)
	{
		return anew Window(specification);
	}

	Window::Window(const WindowSpecification& specification)
	{
		m_Data.Width = specification.Width;
		m_Data.Height = specification.Height;
		m_Data.Fullscreen = specification.Fullscreen;
		m_Data.Maximized = specification.Maximized;
		m_Data.Minimized = specification.Minimized;
		m_Data.Title = specification.Title;
		m_Data.EventCallback = specification.EventCallback;

		Utils::RegisterWindowClass();

		uint32 Width = m_Data.Width;
		uint32 Height = m_Data.Height;
		DWORD ExStyle = 0;
		DWORD Style = 0;

		if (!m_Data.Fullscreen)
		{
			Style |= WS_BORDER | WS_CAPTION;
			Style |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

			if (m_Data.Width == 0 || m_Data.Height == 0)
			{
				HMONITOR Monitor = Utils::GetPrimaryMonitor();
				Utils::MonitorInfo PrimaryMonitor;
				Utils::GetMonitorInfo(Monitor, PrimaryMonitor);

				if (m_Data.Width == 0)
				{
					Width = PrimaryMonitor.Width;
				}
				if (m_Data.Height == 0)
				{
					Height = PrimaryMonitor.Height;
				}
			}
		}
		else
		{
			Style |= WS_POPUP | WS_CLIPSIBLINGS;

			HMONITOR Monitor = Utils::GetPrimaryMonitor();
			Utils::MonitorInfo PrimaryMonitor;
			Utils::GetMonitorInfo(Monitor, PrimaryMonitor);

			Width = PrimaryMonitor.Width;
			Height = PrimaryMonitor.Height;
		}

		m_Data.NativeHandle = CreateWindowEx(
			ExStyle,
			s_WindowClassName,
			m_Data.Title.as<wchar_t>().c_str(),
			Style, 
			0, 0,
			Width, Height,
			NULL, NULL, (HINSTANCE)Platform::WindowsGetInstance(), NULL
		);

		if (m_Data.Maximized)
		{
			ShowWindow((HWND)m_Data.NativeHandle, SW_SHOWMAXIMIZED);
		}
		else if (m_Data.Minimized)
		{
			ShowWindow((HWND)m_Data.NativeHandle, SW_SHOWMINIMIZED);
		}
		else
		{
			ShowWindow((HWND)m_Data.NativeHandle, SW_SHOW);
		}

		if (m_Data.Fullscreen)
		{
			SetWindowPos((HWND)m_Data.NativeHandle, HWND_TOP, 0, 0, Width, Height, SWP_NOZORDER);
		}
	}

	Window::~Window()
	{
		s_CurrentUpdatingWindow = this;
		DestroyWindow((HWND)m_Data.NativeHandle);
		s_CurrentUpdatingWindow = nullptr;
	}

	void Window::SetMaximized(bool Maximized)
	{
		m_Data.Maximized = Maximized;
		s_CurrentUpdatingWindow = this;
		ShowWindow((HWND)m_Data.NativeHandle, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
		s_CurrentUpdatingWindow = nullptr;
	}

	void Window::SetMinimized(bool Minimized)
	{
		m_Data.Minimized = Minimized;
		s_CurrentUpdatingWindow = this;
		ShowWindow((HWND)m_Data.NativeHandle, Minimized ? SW_SHOWMINIMIZED : SW_SHOWNORMAL);
		s_CurrentUpdatingWindow = nullptr;
	}

	void Window::SetTitle(astl::string Title)
	{
		m_Data.Title = Title;
		s_CurrentUpdatingWindow = this;
		SetWindowText((HWND)m_Data.NativeHandle, m_Data.Title.as<wchar_t>().c_str());
		s_CurrentUpdatingWindow = nullptr;
	}

	void Window::SetVSync(bool VSync)
	{
		m_Data.VSync = VSync;
		s_CurrentUpdatingWindow = this;
		s_CurrentUpdatingWindow = nullptr;
	}

	void Window::UpdateWindow()
	{
		s_CurrentUpdatingWindow = this;

		MSG Message = {};
		while (PeekMessage(&Message, (HWND)m_Data.NativeHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		s_CurrentUpdatingWindow = nullptr;
	}

	namespace Utils {

		static Key ConvertWin32KeyCodeToApricot(WPARAM Win32Code)
		{
			// Win32 Codes '0' - '9' are the same as ASCII codes => the same as Apricot codes
			if ('0' <= Win32Code && Win32Code <= '9')
			{
				return (Key)Win32Code;
			}

			// Win32 Codes 'A' - 'Z' are the same as ASCII codes => the same as Apricot codes
			if ('A' <= Win32Code && Win32Code <= 'Z')
			{
				return (Key)Win32Code;
			}

			switch (Win32Code)
			{
				case VK_SPACE:      return Key::Space;
				case VK_SHIFT:      return Key::Shift;
				case VK_CONTROL:    return Key::Control;
				case VK_MENU:       return Key::Alt;
				case VK_TAB:        return Key::Tab;
				case VK_CAPITAL:    return Key::CapsLock;
				case VK_ESCAPE:     return Key::Escape;
				case VK_BACK:       return Key::Backspace;
				case VK_RETURN:     return Key::Enter;
				case VK_DELETE:     return Key::Escape;
				case VK_LEFT:       return Key::ArrowLeft;
				case VK_RIGHT:      return Key::ArrowRight;
				case VK_UP:         return Key::ArrowUp;
				case VK_DOWN:       return Key::ArrowDown;

				case VK_OEM_PLUS:   return Key::Plus;
				case VK_OEM_COMMA:  return Key::Plus;
				case VK_OEM_MINUS:  return Key::Plus;
				case VK_OEM_PERIOD: return Key::Plus;

				case VK_NUMPAD0:    return Key::NumpadZero;
				case VK_NUMPAD1:    return Key::NumpadOne;
				case VK_NUMPAD2:    return Key::NumpadTwo;
				case VK_NUMPAD3:    return Key::NumpadThree;
				case VK_NUMPAD4:    return Key::NumpadFour;
				case VK_NUMPAD5:    return Key::NumpadFive;
				case VK_NUMPAD6:    return Key::NumpadSix;
				case VK_NUMPAD7:    return Key::NumpadSeven;
				case VK_NUMPAD8:    return Key::NumpadEight;
				case VK_NUMPAD9:    return Key::NumpadNine;
								    
				case VK_F1:         return Key::F1;
				case VK_F2:         return Key::F2;
				case VK_F3:         return Key::F3;
				case VK_F4:         return Key::F4;
				case VK_F5:         return Key::F5;
				case VK_F6:         return Key::F6;
				case VK_F7:         return Key::F7;
				case VK_F8:         return Key::F8;
				case VK_F9:         return Key::F9;
				case VK_F10:        return Key::F10;
				case VK_F11:        return Key::F11;
				case VK_F12:        return Key::F12;
			}

			AE_CORE_WARN_TAG("INPUT", "Unreconized Win32 KeyCode! '{}'"/*, Win32Code*/);
			return Key::None;
		}
		
	}

	static LRESULT WindowEventProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
			///////////  KeyPressedEvent  ///////////
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				Key KeyCode = Utils::ConvertWin32KeyCodeToApricot(wParam);
				uint32 RepeatCount = lParam & (0xffff);
				KeyPressedEvent E(KeyCode, RepeatCount, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}

			///////////  KeyReleasedEvent  ///////////
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Key KeyCode = Utils::ConvertWin32KeyCodeToApricot(wParam);
				KeyReleasedEvent E(KeyCode, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}

			///////////  MouseButtonPressedEvent  ///////////
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent E(Button::Left, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent E(Button::Right, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent E(Button::Middle, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}

			///////////  MouseButtonReleasedEvent  ///////////
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent E(Button::Left, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent E(Button::Right, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent E(Button::Middle, s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}

			///////////  WindowEvents  ///////////
			case WM_DESTROY:
			{
				s_CurrentUpdatingWindow->ScheduleClose();
				WindowClosedEvent E(s_CurrentUpdatingWindow->GetUUID());
				s_CurrentUpdatingWindow->GetData().EventCallback(E);
				return 0;
			}
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

}