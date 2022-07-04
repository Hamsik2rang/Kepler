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

		D3D_FEATURE_LEVEL			m_featureLevel;

		HWND m_hWnd;
		bool m_bVSync;

	public:
		DX11Context(const HWND hWnd);

		inline virtual ID3D11Device* GetDevice() override { return m_pDevice; }
		inline virtual ID3D11DeviceContext* GetDeviceContext() override { return m_pImmediateContext; }
		inline virtual ID3D11RenderTargetView* GetRenderTargetView() override { return m_pRenderTargetView; }
		inline virtual IDXGISwapChain* GetSwapChain() override { return m_pSwapChain; }

		// Inherited via IRenderer
		~DX11Context();
		virtual bool Init(const WindowData& data) override;
		virtual void Cleanup() override;
		virtual void SwapBuffer() override;
	};

	// DX11 ���ۿ� �ʿ��� ��ġ ����̽��� ����̽����ؽ�Ʈ�� ��� �Լ��Դϴ�.
	// �Լ��� ȣ��Ǹ� Out �Ķ���͵鿡 �ش� ��ġ���� �޸� �ּҰ� ����˴ϴ�.
	// �Ķ���Ϳ� nullptr�� ������ ���, �ش� ��ü�� �ҷ����� �ʽ��ϴ�.
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