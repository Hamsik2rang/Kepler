#include "kepch.h"

#include "DX11API.h"
#include "Renderer/GraphicsContext.h"
#include "Platform/DirectX11/DX11Context.h"
#include "Core/Application.h"

namespace kepler {

DX11API::DX11API()
	: m_clearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
{
	Init();
}

void DX11API::Init()
{
	auto& window = Application::Get()->GetWindow();

	SetViewport(window.GetWidth(), window.GetHeight());
}

void DX11API::SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth)
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	ID3D11DeviceContext* pDeviceContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
	pDeviceContext->RSSetViewports(1, &viewport);
}

void DX11API::DrawIndexed(const std::shared_ptr<IVertexArray>& pVertexArray)
{
	ID3D11DeviceContext* pImmediateContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
	ID3DUserDefinedAnnotation* anno = nullptr;
	pImmediateContext->QueryInterface(__uuidof(anno), reinterpret_cast<void**>(&anno));
	anno->BeginEvent(L"Test Pass");
	pVertexArray->Bind();
	pImmediateContext->DrawIndexed(pVertexArray->GetIndexBuffer()->GetCount(), 0, 0);
	anno->EndEvent();
}
	
void DX11API::DrawIndexedInstanced(const std::shared_ptr<IVertexArray>& pVertexArray, const std::shared_ptr<IInstanceBuffer>& pInstanceBuffer)
{
	ID3D11DeviceContext* pContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());
	ID3DUserDefinedAnnotation* anno = nullptr;
	pContext->QueryInterface(__uuidof(anno), reinterpret_cast<void**>(&anno));
	anno->BeginEvent(L"Test Pass");
	pVertexArray->Bind();
	pInstanceBuffer->Bind();
	pContext->DrawIndexedInstanced(pVertexArray->GetIndexBuffer()->GetCount(), pInstanceBuffer->GetCount(), 0, 0, 0);
	anno->EndEvent();
}
}