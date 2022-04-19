#include "aepch.h"

#include "Apricot/Core/Window.h"
#include "Apricot/Core/Platform.h"
#include "Apricot/Core/Application.h"

#include "WindowsHeaders.h"

namespace Apricot {

	struct WindowsWindowData
	{
		HWND Handle;
	};

	static const wchar_t* s_WindowClassName = L"Apricot Window Class";

	static LRESULT WindowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

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
			WndClass.lpfnWndProc = WindowProcedure;
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
		DestroyWindow((HWND)m_Data.NativeHandle);
	}

	void Window::SetMaximized(bool Maximized)
	{
		m_Data.Maximized = Maximized;
		ShowWindow((HWND)m_Data.NativeHandle, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
	}

	void Window::SetMinimized(bool Minimized)
	{
		m_Data.Minimized = Minimized;
		ShowWindow((HWND)m_Data.NativeHandle, Minimized ? SW_SHOWMINIMIZED : SW_SHOWNORMAL);
	}

	void Window::SetTitle(astl::string Title)
	{
		m_Data.Title = Title;
		SetWindowText((HWND)m_Data.NativeHandle, m_Data.Title.as<wchar_t>().c_str());
	}

	void Window::SetVSync(bool VSync)
	{
		m_Data.VSync = VSync;
	}

	void Window::UpdateWindow()
	{
		MSG Message;
		while (PeekMessage(&Message, (HWND)m_Data.NativeHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	static LRESULT WindowProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
			case WM_DESTROY:
			{
				Application::Quit(0);
			}
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

}