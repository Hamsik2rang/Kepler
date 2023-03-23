// For use by client
#pragma once

//----- Core ------------------------------
#include "Core/Log.h"
#include "Core/Application.h"
#include "Core/Event/Event.hpp"
#include "Core/Layer.hpp"
#include "Core/Input.h"
#include "Core/InputCode.h"


//----- Scene -----------------------------
#include "Core/Scene/Entity.hpp"
#include "Core/Scene/Component.hpp"
#include "Core/Scene/Scene.h"
#include "Core/Scene/ScriptableEntity.h"
//-----------------------------------------

//----- Renderer --------------------------
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderState.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Renderer/PerspectiveCamera.h"
#include "Core/Renderer/EditorCamera.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/RenderProfiler.h"
//-----------------------------------------

//----- Audio -----------------------------
#include "Core/Audio/Audio.h"
#include "Core/Audio/AudioSource.hpp"
//-----------------------------------------

//-----------------------------------------


//----- Math ------------------------------
#include "KeplerMath.h"
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