// For use by client
#pragma once

//----- Core ------------------------------
#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Event/Event.hpp"
//-----------------------------------------

//----- Platform --------------------------
#include "Platform/Windows/WinAPI.h"
#include "Platform/Windows/WindowsWindow.h"
//-----------------------------------------

//----- Global Variables-------------------
namespace kepler {
	HINSTANCE	g_hInst;
	int			g_nCmdShow;
}
//-----------------------------------------


//----- Entry Point -----------------------
#ifdef UNICODE
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
// NOTICE: �ݵ�� ���� �������� �����ؾ� �մϴ�!!
#include "Core/EntryPoint.h"
//-----------------------------------------