#include "kepch.h"

#include "DX11Context.h"
#include "Core/Renderer/RenderState.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"

kepler::DX11Context::DX11Context(const HWND hWnd)
	: m_hWnd{ hWnd }
	, m_bVSync{ false }
	, m_pDevice{ nullptr }
	, m_pDeviceContext{ nullptr }
	, m_pSwapChain{ nullptr }
	, m_featureLevel{ D3D_FEATURE_LEVEL_11_0 }
{

}

void kepler::DX11Context::Cleanup()
{
	// ���� �� ������ ���� �������� ������ ���� ü���� ���� �� �� ���ܰ� �߻��մϴ�.
	if (m_pSwapChain) { m_pSwapChain->SetFullscreenState(false, nullptr); }
	if (m_pDeviceContext) { m_pDeviceContext->Release(); m_pDeviceContext = nullptr; }
	if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
	if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
}

kepler::DX11Context::~DX11Context()
{
	Cleanup();
}

bool kepler::DX11Context::Init(const WindowData& data)
{
	m_bVSync = data.bVSync;

	DXGI_SWAP_CHAIN_DESC scDesc{};
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.Width = 0;	// (Im YongsiK) Use DXGI_SCALING_NONE ?
	scDesc.BufferDesc.Height = 0;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = data.bVSync ? 60 : 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = m_hWnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	// Debug ����� ���� ����� ����̽� �÷��� ����
#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT featureLevelCount = ARRAYSIZE(featureLevels);

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels,
		featureLevelCount,
		D3D11_SDK_VERSION,
		&scDesc,
		&m_pSwapChain,
		&m_pDevice,
		&m_featureLevel,
		&m_pDeviceContext);

	if (FAILED(hr))
	{
		KEPLER_ASSERT(false, "Fail to Initialize DirectX11 API");
		return false;
	}

	return true;
}

void kepler::DX11Context::SwapBuffer()
{
	m_pSwapChain->Present((m_bVSync ? 1 : 0), 0);
}