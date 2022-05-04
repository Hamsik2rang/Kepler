#include "kepch.h"

#include "DX11API.h"
#include "Renderer/GraphicsContext.h"

void kepler::DX11API::Init()
{

}

void kepler::DX11API::ClearColor()
{
	static float color[]{ 0.1f, 0.1f, 0.1f, 1.0f };
	ID3D11DeviceContext* pImmediateContext = IGraphicsContext::Get()->GetDeviceContext();
	ID3D11RenderTargetView* pRenderTargetView = IGraphicsContext::Get()->GetRenderTargetView();
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);
}

void kepler::DX11API::SetColor()
{

}

void kepler::DX11API::SetViewport()
{

}

void kepler::DX11API::Resize(uint32_t width, uint32_t height)
{

}

void kepler::DX11API::DrawIndexed()
{

}
