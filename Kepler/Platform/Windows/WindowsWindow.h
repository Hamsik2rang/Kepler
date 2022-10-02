#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Platform/Win32/WinAPI.h"


namespace kepler {
	// Window�� ���� �Ӽ���� �ݹ� �Լ��� ����ϴ�. Init���� �ʱ�ȭ�ǰ� WinAPI.cpp�� WndProc���� �̺�Ʈ�� �ڵ鸵�� �� ���� �����˴ϴ�.
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
		virtual void GetCursorPos(uint32_t& outXPos, uint32_t& outYPos) override;
		inline virtual uint32_t GetWidth() const override { return m_data.width; }
		inline virtual uint32_t GetHeight() const override { return m_data.height; }

		inline virtual void SetVSync(bool isEnabled) override { m_data.bVSync = isEnabled; };
		inline virtual bool IsVSync() const override { return m_data.bVSync; }
		inline virtual HWND GetWindowHandle() const override { return m_hWnd; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
	};
}