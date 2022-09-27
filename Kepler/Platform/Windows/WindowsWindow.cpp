#include "kepch.h"

#include "WindowsWindow.h"
#include "Core/Renderer/GraphicsContext.h"
#include "Core/Renderer/Renderer.h"

namespace kepler {
	extern HINSTANCE	g_hInst;
	extern int			g_nCmdShow;

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
		m_data.bVSync = true;

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
		IGraphicsContext::Create(&m_hWnd);
		if (!IGraphicsContext::Get()->Init(m_data))
		{
			IGraphicsContext::Get()->Cleanup();
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize DirectX Device - {0} {1}", __FILE__, __LINE__);
			KEPLER_ASSERT(false, "Can't Initialize Window Instance - See Core Log for more info.");
		}

		kepler::ShowWindow(m_hWnd);
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_data));

		SetVSync(true);
	}

	void WindowsWindow::GetCursorPos(uint32_t& outXPos, uint32_t& outYPos)
	{
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		::ScreenToClient(m_hWnd, &cursorPos);
		outXPos = cursorPos.x;
		outYPos = cursorPos.y;
	};


	void WindowsWindow::Shutdown()
	{
		// TODO: �����ؾ� �� ������ ���ҽ��� �ִٸ� �� ������ �����մϴ�.
		IGraphicsContext::Get()->Cleanup();
	}

	void WindowsWindow::OnUpdate()
	{
		// TODO: Update Loop���� �ؾ��� �۾��� �ۼ�
		
		// swap back buffer and front buffer
		IGraphicsContext::Get()->SwapBuffer();
	}
}
