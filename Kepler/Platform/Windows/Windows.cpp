#include "kepch.h"

#include "Windows.h"

namespace kepler {
	HWND Windows::s_hMainWnd = nullptr;
	int Windows::s_windowCount = 0;

	IWindow* IWindow::Create(const WindowProps& props)
	{
		return new Windows(props);
	}

	Windows::Windows(const WindowProps& props)
	{
		Init(props);
	}

	Windows::~Windows()
	{
		Shutdown();
	}

	void Windows::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		KEPLER_CORE_INFO("Creating Window {0} ({1}, {2})", props.title, props.width, props.height);
		ATOM result = RegisterWindowClass(m_data.title, kepler::WndProc);
		KEPLER_CORE_INFO("Register window result is {0}", result);
		if (!result)
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Main Window");
			throw;
		}
		m_hWnd = kepler::InitInstance(m_data.title, m_data.width, m_data.height);
		if (m_hWnd)
		{
			s_windowCount++;
			if (!::SetWindowLongPtrW(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_data)))
			{
				if (::GetLastError() != 0)
				{
					MessageBox(nullptr, L"Can't set window handle data", L"Error", MB_OK);
					KEPLER_ASSERT(false, __FILE__, __LINE__);
				}
			}
		}
		else
		{
			KEPLER_CORE_CRITICAL("CRITICAL: Can't Initialize Window Instance");
		}
		if (!s_hMainWnd)
		{
			s_hMainWnd = m_hWnd;
		}
		
		SetVSync(true);
	}

	void Windows::Shutdown()
	{
		// TODO:핸들 반납하기
	}

	void Windows::OnUpdate()
	{

	}

	void Windows::SetVSync(bool isEnabled)
	{
		if (isEnabled)
		{
			m_data.bVSync = true;
			// TODO: directx vsync 세팅
		}
		else
		{
			m_data.bVSync = false;
			// TODO: directx vsync 세팅
		}
	}


	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
	}

	INT_PTR CALLBACK About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (msg)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}
}
