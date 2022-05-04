#include "kepch.h"
#include "DX11Context.h"

kepler::DX11Context::DX11Context(const HWND hWnd)
    :m_hWnd{hWnd}
    ,m_bVSync{false}
    ,m_pDevice{nullptr}
    ,m_pImmediateContext{nullptr}
    ,m_pSwapChain{nullptr}
    ,m_pRenderTargetView{nullptr}
{

}

void kepler::DX11Context::Cleanup()
{
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pImmediateContext) { m_pImmediateContext->Release(); m_pImmediateContext = nullptr; }
    if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
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
    scDesc.BufferDesc.Width = data.width;
    scDesc.BufferDesc.Height = data.height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
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

    // 추후 feature level 11.1 까지 지원할 것
    const D3D_FEATURE_LEVEL featureLevels[]{
        /*D3D_FEATURE_LEVEL_11_1,*/
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT featureLevelCount = ARRAYSIZE(featureLevels);

    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain(
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
        &m_pImmediateContext);

    if (FAILED(hr))
    {
        KEPLER_ASSERT(false, "Fail to Initialize DirectX11 API");
        return false;
    }


    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create back buffer");
        return false;
    }

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    if (FAILED(hr))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create RenderTarget View");
        return false;
    }
    pBackBuffer->Release();

    return true;
}

void kepler::DX11Context::SwapBuffer()
{
    m_pSwapChain->Present((m_bVSync ? 1 : 0), 0);
}