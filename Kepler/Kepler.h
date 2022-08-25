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

//----- Renderer --------------------------
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderState.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/GraphicsAPI.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderProfiler.h"
//-----------------------------------------

//----- Audio -----------------------------
#include "Audio/Audio.h"
#include "Audio/AudioSource.hpp"
//-----------------------------------------

//----- Math ------------------------------
#include "KeplerMath.h"
//-----------------------------------------

//----- Scene -----------------------------
#include "Scene/Entity.h"
#include "Scene/Component.hpp"
#include "Scene/Scene.h"
//-----------------------------------------

//----- Platform --------------------------
#include "Platform/Win32/WinAPI.h"
#include "Platform/Windows/WindowsWindow.h"
#include "ImGui/ImGuiLayer.h"
#include "imgui.h"
#include "ImPlot/implot.h"
//-----------------------------------------

//----- Utility --------------------------
#include "Utility/Timer.hpp"
#include "Utility/Random.hpp"
//-----------------------------------------

//----- Global Variables-------------------
namespace kepler {
	extern HINSTANCE	g_hInst;
	extern int			g_nCmdShow;
}
//-----------------------------------------

//----- Entry Point -----------------------
#ifdef UNICODE
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
//-----------------------------------------