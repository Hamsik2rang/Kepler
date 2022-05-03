#pragma once

#include "Renderer/GraphicsContext.h"
#include <Windows.h>
#include <d3d11.h>


namespace kepler {

	class DX11Context : public GraphicsContext
	{
	private:
		static DX11Context*		s_pInstance;
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pImmediateContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		IDXGISwapChain*			m_pSwapChain;

		HWND m_hWnd;
		bool m_bVSync;
	public:
		DX11Context(const HWND hWnd);

		// Inherited via IRenderer
		~DX11Context();
		virtual bool Init() override;
		virtual void Cleanup() override;
		virtual void SwapBuffer(bool bVSync) override;

		inline static DX11Context* Get() { return s_pInstance; }
		inline ID3D11Device* GetDevice() { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
		inline ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }
		inline IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
	};
}