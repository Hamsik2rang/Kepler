#include "kepch.h"

#include "DX11Context.h"
#include "DX11Debug.h"
#include "Renderer/RenderState.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace kepler {

DX11Context::DX11Context(const HWND hWnd)
	: m_hWnd{ hWnd }
	, m_bVSync{ false }
	, m_pDevice{ nullptr }
	, m_pDeviceContext{ nullptr }
	, m_pSwapChain{ nullptr }
	, m_pDebugger{ nullptr }
	, m_featureLevel{ D3D_FEATURE_LEVEL_11_0 }
{

}

void DX11Context::Cleanup()
{
	// 종료 전 윈도우 모드로 설정하지 않으면 스왑 체인을 해제 할 때 예외가 발생합니다.
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, nullptr);
	}

	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = nullptr;
	}
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
	if (m_pDebugger)
	{
		m_pDebugger->CheckReferenceCount();
		delete m_pDebugger;
		m_pDebugger = nullptr;
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

DX11Context::~DX11Context()
{
	Cleanup();
}

bool DX11Context::Init(const WindowData& data)
{
	m_bVSync = data.bVSync;

	// Start Init Adapter Info
	IDXGIFactory* pFactory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
	{
		KEPLER_ASSERT(false, "Fail to Initialize IDXGIFactory");
		return false;
	}
	IDXGIAdapter* pAdapter = nullptr;
	if (FAILED(pFactory->EnumAdapters(0, &pAdapter)))
	{
		KEPLER_ASSERT(false, "Fail to Initialize IDXGIAdapter");
		return false;
	}
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(pAdapter->GetDesc(&adapterDesc)))
	{
		KEPLER_ASSERT(false, "Fail to Get DXGI Adapter Description(DXGI_ADAPTER_DESC)");
		return false;
	}
	
	int vendorId = adapterDesc.VendorId;
	int videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	char videoCardDescription[128] = { 0, };
	size_t stringLength = 0;
	if (wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		KEPLER_ASSERT(false, "Fail to Get Video Card Description");
		return false;
	}

	// Logging Adapter Info
	KEPLER_CORE_INFO("Video Card Vendor Id : {0}", vendorId);
	KEPLER_CORE_INFO("Video Card Memory : {0}", videoCardMemory);
	KEPLER_CORE_INFO("Video Card Description : {0}", videoCardDescription);

	pAdapter->Release();
	pAdapter = nullptr;
	pFactory->Release();
	pFactory = nullptr;
	// End Init Adapter Info

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
	// Debug 모드일 때만 디버그 디바이스 플래그 세팅
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

#ifdef _DEBUG
	m_pDebugger = new DX11Debug;
	m_pDebugger->Init();
#endif

	return true;
}

void DX11Context::SwapBuffer()
{
	m_pSwapChain->Present((m_bVSync ? 1 : 0), 0);
}
}