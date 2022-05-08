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

		XMMATRIX m_projectionMatrix{};
		XMMATRIX m_worldMatrix{};
		XMMATRIX m_orthoMatrix{};

	public:
		DX11Context(const HWND hWnd);

		inline ID3D11Device* GetDevice() { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
		inline ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }
		inline IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
		inline void GetProjectionMatrix(XMMATRIX& projectionMatrix) { projectionMatrix = m_projectionMatrix; };
		inline void GetWorldMatrix(XMMATRIX& worldMatrix) { worldMatrix = m_worldMatrix; };
		inline void GetOrthoMatrix(XMMATRIX& orthoMatrix) { orthoMatrix = m_orthoMatrix; };

		// Inherited via IRenderer
		~DX11Context();
		virtual bool Init(const WindowData& data) override;
		// ���� ���۸� �ʱ�ȭ�մϴ�.
		bool InitDepthBuffer(const WindowData& data);
		// ���� ���ٽ��� �ʱ�ȭ�մϴ�.
		bool InitDepthStencil(const WindowData& data);
		virtual void Cleanup() override;
		virtual void SwapBuffer() override;
		// 2D �������� ������ ���� ZBuffer�� �մϴ�.
		void TurnZBufferOn();
		// 2D �������� ���� �� ZBuffer�� ���ϴ�.
		void TurnZBufferOff();
	};
}