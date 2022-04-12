#include "kepch.h"

#include "WindowsWindow.h"

namespace kepler {
	extern HINSTANCE	g_hInst;
	extern int			g_nCmdShow;

	HWND WindowsWindow::s_hMainWnd = nullptr;
	int WindowsWindow::s_windowCount = 0;

	IWindow* IWindow::Create(const WindowProperty& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperty& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperty& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		KEPLER_CORE_INFO("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);
		// ������ Ŭ���� ���
		ATOM result = RegisterWindowClass(m_data.title, kepler::WndProc);
		KEPLER_CORE_INFO("Register window result is {0}", result);
		if (!result)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Main Window");
			KEPLER_ASSERT(false, "Can't Initialize Main Window");
		}
		// ������ Ŭ���� �ʱ�ȭ �� �̺�Ʈ �ڵ鸵�� ���� ���� ������ ���
		m_hWnd = kepler::InitInstance(m_data.title, m_data.width, m_data.height, reinterpret_cast<LONG_PTR>(&m_data));
		if (!m_hWnd)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Window Instance - {0} {1}", __FILE__, __LINE__);
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See Core Log for more info.");
		}
		// D3D Device ����
		if (!CreateD3DDevice(m_hWnd, &m_pDevice, &m_pImmediateContext, &m_pSwapChain, &m_pRenderTargetView))
		{
			CleanupD3DDevice();
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize DirectX Device - {0} {1}", __FILE__, __LINE__);
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See Core Log for more info.");
		}

		kepler::ShowWindow(m_hWnd);
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_data));
		
		s_windowCount++;
		if (!s_hMainWnd)
		{
			s_hMainWnd = m_hWnd;
		}
		
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		// TODO: �����ؾ� �� ������ ���ҽ��� �ִٸ� �� ������ �����մϴ�.
		CleanupD3DDevice();
	}

	void WindowsWindow::CleanupD3DDevice()
	{
		if (m_pRenderTargetView)	{ m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
		if (m_pSwapChain)			{ m_pSwapChain->Release(); m_pSwapChain = nullptr; }
		if (m_pImmediateContext)	{ m_pImmediateContext->Release(); m_pImmediateContext = nullptr; }
		if (m_pDevice)				{ m_pDevice->Release(); m_pDevice = nullptr; }
	}

	void WindowsWindow::OnUpdate()
	{
		// TODO: Update Loop���� �ؾ��� �۾��� �ۼ�
		m_pSwapChain->Present((m_data.bVSync ? 1 : 0), 0);
	}

	void WindowsWindow::ClearRender()
	{
		static const float clearColor[]{ 0.1f, 0.1f, 0.1f, 1.0f };
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	}
}
