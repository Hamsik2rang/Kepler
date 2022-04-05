// For use by client
#pragma once



//----- Core ------------------------------
#include "Core/Log.h"
#include "Core/Application.h"
//-----------------------------------------


//----- Entry Point -----------------------
#ifdef UNICODE
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#include "Core/EntryPoint.h"
//-----------------------------------------