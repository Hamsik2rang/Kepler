// For use by client
#pragma once

//----- Core ------------------------------
#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Event/Event.hpp"
#include "Core/Layer.hpp"
#include "Core/Input.h"
#include "Core/InputCode.h"
//-----------------------------------------

//----- Math ------------------------------
#include "Math/Vector.h"

//-----------------------------------------

//----- Platform --------------------------
#include "Platform/Win32/WinAPI.h"
#include "Platform/Windows/WindowsWindow.h"
#include "ImGui/ImGuiLayer.h"
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
#ifndef KEPLER_UNITTEST
#include "Core/EntryPoint.h"
#endif
//-----------------------------------------