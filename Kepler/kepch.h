// Kepler Precompiled header
// 프로젝트 안에서 사용되는 모든 stl이나 외부 API들은 여기 선언합니다.
// 모든 .cpp 파일에서 최상단에 #include "kepch.h"를 작성해 pch를 참조하도록 합시다.

#pragma once

#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <memory>

#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

// for Windows
#ifdef KEPLER_PLATFORM_WINDOWS
#include <Windows.h>

#include "framework.h"
#include "resource.h"

#endif
// for DirectX
#ifdef KEPLER_GRAPHICS_API_DIRECTX
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#endif

// DirectXTK
#include "BufferHelpers.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"