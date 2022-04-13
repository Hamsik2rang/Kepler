#include "kepch.h"

#include "DirectXAPI.h"

bool kepler::CreateD3DDevice(HWND hWnd, 
                             ID3D11Device** pDevice, 
                             ID3D11DeviceContext** pImmediateContext, 
                             IDXGISwapChain** pSwapChain, 
                             ID3D11RenderTargetView** pRenderTargetView)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[]
    { 
        D3D_FEATURE_LEVEL_11_0, 
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
        &sd,
        pSwapChain,
        pDevice,
        &featureLevel,
        pImmediateContext);

    if (FAILED(hr))
    {
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = (*pSwapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    hr = (*pDevice)->CreateRenderTargetView(pBackBuffer, nullptr, pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}