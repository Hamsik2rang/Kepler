#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

#include "Platform/Win32/WinAPI.h"
#include "Platform/DirectX/DirectXAPI.h"


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

	class KEPLER_API WindowsWindow : public IWindow
	{
	private:
		static HWND s_hMainWnd;
		HWND		m_hWnd = nullptr;

		// �ӽ�. ���Ŀ� �������� ����� �� �� ������ ����̽��� ������ �� ����
		ID3D11Device*			m_pDevice = nullptr;
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
		IDXGISwapChain*			m_pSwapChain = nullptr;

		WindowData	m_data{};

		virtual void Init(const WindowProperty& props);
		virtual void Shutdown();
		
		// ���������� �ӽ�
		void CleanupD3DDevice();

	public:
		static int s_windowCount;

		WindowsWindow(const WindowProperty& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void ClearRender() override;
		
		virtual inline uint32_t GetWidth() const override { return m_data.width; }
		virtual inline uint32_t GetHeight() const override { return m_data.height; }

		virtual inline void SetVSync(bool isEnabled) override { m_data.bVSync = isEnabled; };
		virtual inline bool IsVSync() const override { return m_data.bVSync; }
		inline HWND GetWindowHandle() const { return m_hWnd; }
		inline ID3D11Device* GetD3DDevice() { return m_pDevice; }
		inline ID3D11DeviceContext* GetD3DDeviceContext() { return m_pImmediateContext; }

		virtual inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; };
		
	};
}