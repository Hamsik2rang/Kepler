#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Platform/Win32/WinAPI.h"


namespace kepler {
	// Window의 현재 속성들과 콜백 함수를 담습니다. Init에서 초기화되고 WinAPI.cpp의 WndProc에서 이벤트가 핸들링될 때 마다 수정됩니다.
	struct WindowData
	{
		std::string title;
		uint32_t	width;
		uint32_t	height;
		bool		bVSync;

		IWindow::EventCallbackFunc eventCallback;
	};

	class WindowsWindow : public IWindow
	{
	private:
		HWND				m_hWnd = nullptr;
		WindowData			m_data{};

		virtual void Init(const WindowProperty& props);
		virtual void Shutdown();
	public:
		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		inline virtual uint32_t GetWidth() const override { return m_data.width; }
		inline virtual uint32_t GetHeight() const override { return m_data.height; }
		

		inline virtual void SetVSync(bool isEnabled) override { m_data.bVSync = isEnabled; };
		inline virtual bool IsVSync() const override { return m_data.bVSync; }
		inline virtual HWND GetWindowHandle() const override { return m_hWnd; }
		
		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
	};
}