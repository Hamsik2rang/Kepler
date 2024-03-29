#pragma once

#include "Core/Renderer/GraphicsContext.h"
#include <Windows.h>
#include <d3d11.h>


namespace kepler {

class DX11Debug;

class DX11Context : public IGraphicsContext
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;

	DX11Debug* m_pDebugger;
	D3D_FEATURE_LEVEL			m_featureLevel;

	HWND m_hWnd;
	bool m_bVSync;

public:
	DX11Context(const HWND hWnd);

	inline virtual void* GetDevice() override { return m_pDevice; }
	inline virtual void* GetDeviceContext() override { return m_pDeviceContext; }
	inline virtual void* GetSwapChain() override { return m_pSwapChain; }

	// Inherited via IRenderer
	~DX11Context();
	virtual bool Init(const WindowData& data) override;
	virtual void Cleanup() override;
	virtual void SwapBuffer() override;
};
}