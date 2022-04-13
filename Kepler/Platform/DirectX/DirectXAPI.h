#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace kepler {

	bool CreateD3DDevice(HWND hWnd, 
						ID3D11Device** pDevice, 
						ID3D11DeviceContext** pImmediateContext, 
						IDXGISwapChain** pSwapChain, 
						ID3D11RenderTargetView** pRenderTargetView);
}