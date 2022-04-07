#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Platform/Windows/WinAPI.h"


namespace kepler {

	struct WindowData
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool bVSync;

		IWindow::EventCallbackFunc eventCallback;
	};

	class KEPLER_API Windows : public IWindow
	{
	private:
		static HWND s_hMainWnd;
		HWND		m_hWnd = nullptr;

		WindowData	m_data;

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	public:
		static int s_windowCount;

		Windows(const WindowProps& props);
		virtual ~Windows();

		virtual void OnUpdate() override;
		virtual inline uint32_t GetWidth() const override { return m_data.width; }
		virtual inline uint32_t GetHeight() const override { return m_data.height; }

		virtual void SetVSync(bool isEnabled) override;
		virtual bool IsVSync() const override { return m_data.bVSync; }
		virtual HWND GetWindowHandle() const override { return m_hWnd; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
	};
}