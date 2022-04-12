#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Platform/Win32/WinAPI.h"
#include "Platform/DirectX/DirectXAPI.h"


namespace kepler {
	// Window의 현재 속성들과 콜백 함수를 담습니다. Init에서 초기화되고 WinAPI.cpp의 WndProc에서 이벤트가 핸들링될 때 마다 수정됩니다.
	struct WindowData
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool bVSync;

		IWindow::EventCallbackFunc eventCallback;
	};

	class KEPLER_API WindowsWindow : public IWindow
	{
	private:
		static HWND s_hMainWnd;
		HWND		m_hWnd = nullptr;

		WindowData	m_data{};

		virtual void Init(const WindowProperty& props);
		virtual void Shutdown();

	public:
		static int s_windowCount;

		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual inline uint32_t GetWidth() const override { return m_data.width; }
		virtual inline uint32_t GetHeight() const override { return m_data.height; }

		virtual void SetVSync(bool isEnabled) override;
		virtual bool IsVSync() const override { return m_data.bVSync; }
		virtual HWND GetWindowHandle() const override { return m_hWnd; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
	};
}