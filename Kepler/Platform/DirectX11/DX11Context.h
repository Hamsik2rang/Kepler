#pragma once

#include "Renderer/GraphicsContext.h"
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>


namespace kepler {

	using namespace DirectX;

	class DX11Context : public IGraphicsContext
	{
	private:
		ID3D11Device*				m_pDevice;
		ID3D11DeviceContext*		m_pImmediateContext;
		IDXGISwapChain*				m_pSwapChain;
		ID3D11RenderTargetView*		m_pRenderTargetView;
		ID3D11Texture2D*			m_pDepthStencilBuffer = nullptr;
		ID3D11DepthStencilState*	m_pDepthStencilState = nullptr;
		ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
		ID3D11DepthStencilState*	m_pDepthDisabledStencilState = nullptr;
		ID3D11RasterizerState*		m_pRasterState = nullptr;

		D3D_FEATURE_LEVEL			m_featureLevel;

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
		// 깊이 버퍼를 초기화합니다.
		bool InitDepthBuffer(const WindowData& data);
		// 깊이 스텐실을 초기화합니다.
		bool InitDepthStencil(const WindowData& data);
		virtual void Cleanup() override;
		virtual void SwapBuffer() override;
		// 2D 렌더링을 시작을 위해 ZBuffer를 켭니다.
		void TurnZBufferOn();
		// 2D 렌더링을 종료 후 ZBuffer를 끕니다.
		void TurnZBufferOff();
	};

	// DX11 조작에 필요한 장치 디바이스와 디바이스컨텍스트를 얻는 함수입니다.
	// 함수가 호출되면 Out 파라미터들에 해당 장치들의 메모리 주소가 저장됩니다.
	// 파라미터에 nullptr을 지정할 경우, 해당 객체는 불러오지 않습니다.
	inline void GetDX11DeviceAndDeviceContext(ID3D11Device** ppOutDevice, ID3D11DeviceContext** ppOutDeviceContext)
	{
		DX11Context* pContext = reinterpret_cast<DX11Context*>(IGraphicsContext::Get());
		if (ppOutDevice)
		{
			*ppOutDevice = pContext->GetDevice();
		}
		if (ppOutDeviceContext)
		{
			*ppOutDeviceContext = pContext->GetDeviceContext();
		}
	}
}