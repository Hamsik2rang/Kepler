#pragma once

#include "Renderer/GraphicsContext.h"
#include <Windows.h>
#include <d3d11.h>


namespace kepler {

	class DX11Context : public IGraphicsContext
	{
	private:
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pImmediateContext;
		IDXGISwapChain*			m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;

		D3D_FEATURE_LEVEL		m_featureLevel;

		HWND m_hWnd;
		bool m_bVSync;

	public:
		DX11Context(const HWND hWnd);

		inline ID3D11Device* GetDevice() { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
		inline ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }
		inline IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }

		// Inherited via IRenderer
		~DX11Context();
		virtual bool Init(const WindowData& data) override;
		virtual void Cleanup() override;
		virtual void SwapBuffer() override;
	};
}