#pragma once

#include <memory>
#include <Windows.h>
#include "Core/Base.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Platform/Windows/WindowsWindow.h"

namespace kepler {

struct WindowData;

class IGraphicsContext
{
private:
	static IGraphicsContext* s_pInstance;

public:
	virtual ~IGraphicsContext();
	virtual bool Init(const WindowData& data) = 0;
	virtual void Cleanup() = 0;
	virtual void SwapBuffer() = 0;

	// Interface for DirectX11 
	virtual void* GetDevice() { return nullptr; }
	virtual void* GetDeviceContext() { return nullptr; }
	virtual void* GetSwapChain() { return nullptr; }

	// TODO: 추후 API사양을 인자로 받아 적절한 컨텍스트를 생성하도록 변경할 것(현재는 DX11만 생성 가능)
	static IGraphicsContext* Create(const void* WindowHandle);

	inline static IGraphicsContext* Get() { return s_pInstance; }
};
}