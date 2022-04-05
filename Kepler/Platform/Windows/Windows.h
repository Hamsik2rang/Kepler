#pragma once

#include "Core/Window.h"

namespace kepler {

	class KEPLER_API Windows : public IWindow
	{
	private:
		HINSTANCE	m_hInst;
		HWND		m_hWnd;
		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
			bool bVSync;

			EventCallbackFunc eventCallback;
		} m_data;

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	public:
		static bool s_bIsInitialized;

		Windows(const WindowProps& props);
		virtual ~Windows();

		virtual void OnUpdate() override;
		virtual inline uint32_t GetWidth() const override { return m_data.width; };
		virtual inline uint32_t GetHeight() const override { return m_data.height; };

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
		virtual void SetVSync(bool isEnabled) override;
		virtual bool IsVSync() const override { return m_data.bVSync; };
	};
}