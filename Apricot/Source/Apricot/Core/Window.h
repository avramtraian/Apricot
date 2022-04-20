#pragma once

#include "Base.h"
#include "Events/Event.h"

namespace Apricot {

	using PFN_WindowEventCallback = void(*)(Event& E);

	struct WindowSpecification
	{
		astl::string Title;
		uint32 Width = 0, Height = 0;
		int32 X = 0, Y = 0;
		bool VSync = true;
		bool Minimized = false;
		bool Maximized = true;
		bool Fullscreen = false;
		bool AllowResizing = true;

		PFN_WindowEventCallback EventCallback = nullptr;
	};

	class APRICOT_API Window
	{
	public:
		struct WindowData
		{
			astl::string Title;
			uint32 Width = 0, Height = 0;
			int32 X = 0, Y = 0;
			bool VSync = true;
			bool Minimized = false;
			bool Maximized = true;
			bool Fullscreen = false;
			bool AllowResizing = true;

			PFN_WindowEventCallback EventCallback = nullptr;

			void* NativeHandle = nullptr;
		};

		static Window* Create(const WindowSpecification& specification);

	private:
		Window(const WindowSpecification& specification);
	public:
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	public:
		void OnWindowMaximized();
		void OnWindowMinimized();

		void SetMaximized(bool Maximized);
		void SetMinimized(bool Minimized);
		void SetTitle(astl::string Title);
		void SetVSync(bool VSync);

		void UpdateWindow();

		UUID GetUUID() const { return m_UUID; }
		WindowData& GetData() { return m_Data; }
		const WindowData& GetData() const { return m_Data; }

		bool ShouldClose() const { return m_ShouldClose; }
		bool IsPrimary() const { return m_Primary; }

		void ScheduleClose() { m_ShouldClose = true; }
		void SetPrimary(bool Primary) { m_Primary = Primary; }

	private:
		WindowData m_Data;
		UUID m_UUID;
		bool m_ShouldClose = false;
		bool m_Primary = false;
	};

}