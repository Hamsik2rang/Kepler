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
		uint32_t	width;
		uint32_t	height;
		bool		bVSync;

		IWindow::EventCallbackFunc eventCallback;
	};

	class WindowsWindow : public IWindow
	{
	private:
		HWND		m_hWnd = nullptr;

		// 임시. 추후에 렌더러가 생기면 그 땐 렌더러 디바이스로 변경할 수 있음
		ID3D11Device*			m_pDevice = nullptr;
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
		IDXGISwapChain*			m_pSwapChain = nullptr;

		WindowData	m_data{};

		virtual void Init(const WindowProperty& props);
		virtual void Shutdown();
		
		// 마찬가지로 임시
		void CleanupD3DDevice();

	public:
		static HWND s_hMainWnd;
		static int s_windowCount;

		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void ClearRender() override;
		
		virtual inline uint32_t GetWidth() const override { return m_data.width; }
		virtual inline uint32_t GetHeight() const override { return m_data.height; }

		virtual inline void SetVSync(bool isEnabled) override { m_data.bVSync = isEnabled; };
		virtual inline bool IsVSync() const override { return m_data.bVSync; }
		inline virtual HWND GetWindowHandle() const override { return m_hWnd; }
		inline ID3D11Device* GetD3DDevice() const override{ return m_pDevice; }
		inline ID3D11DeviceContext* GetD3DDeviceContext() const override{ return m_pImmediateContext; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
		
	};
}