#include "kepch.h"

#include "DX11API.h"
#include "Renderer/GraphicsContext.h"

void kepler::DX11API::Init()
{
	SetViewport();
	ClearColor();
}

void kepler::DX11API::ClearColor()
{
	static float color[]{ 0.1f, 0.1f, 0.1f, 1.0f };
	SetColor(color);
}

void kepler::DX11API::SetColor(const float color[4])
{
	ID3D11DeviceContext* pImmediateContext = IGraphicsContext::Get()->GetDeviceContext();
	ID3D11RenderTargetView* pRenderTargetView = IGraphicsContext::Get()->GetRenderTargetView();
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);
}

void kepler::DX11API::SetViewport()
{
	float screenWidth = 800;
	float screenHeight = 600;
	Resize(screenWidth, screenHeight);
}

void kepler::DX11API::Resize(uint32_t width, uint32_t height)
{
	// D3D11_VIEWPORT�� �����մϴ�
	D3D11_VIEWPORT viewport{};
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	// pImmediateContext�� ����Ʈ�� �����մϴ�
	ID3D11DeviceContext* pImmediateContext = IGraphicsContext::Get()->GetDeviceContext();
	pImmediateContext->RSSetViewports(1, &viewport);
}

void kepler::DX11API::DrawIndexed()
{

}