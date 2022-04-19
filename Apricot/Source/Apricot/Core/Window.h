#pragma once

#include "Base.h"

namespace Apricot {

	struct WindowSpecification
	{
		astl::string Title;
		uint32 Width = 0, Height = 0;
		bool VSync = true;
		bool Minimized = false;
		bool Maximized = true;
		bool Fullscreen = false;
	};

	class APRICOT_API Window
	{
	public:
		struct WindowData
		{
			astl::string Title;
			uint32 Width = 0, Height = 0;
			bool VSync = true;
			bool Minimized = false;
			bool Maximized = true;
			bool Fullscreen = false;

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
		void SetMaximized(bool Maximized);
		void SetMinimized(bool Minimized);
		void SetTitle(astl::string Title);
		void SetVSync(bool VSync);

		void UpdateWindow();

		UUID GetUUID() const { return m_UUID; }

	private:
		WindowData m_Data;
		UUID m_UUID;
	};

}