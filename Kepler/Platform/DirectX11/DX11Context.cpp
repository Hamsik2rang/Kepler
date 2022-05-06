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
    // ���� �� ������ ���� �������� ������ ���� ü���� ���� �� �� ���ܰ� �߻��մϴ�.
    if (m_pSwapChain) { m_pSwapChain->SetFullscreenState(false, NULL); }

    if (m_pDepthDisabledStencilState) { m_pDepthDisabledStencilState->Release(); m_pDepthDisabledStencilState = nullptr; }
    if (m_pRasterState) { m_pRasterState->Release(); m_pRasterState = 0; }
    if (m_pDepthStencilView) { m_pDepthStencilView->Release(); m_pDepthStencilView = nullptr; }
    if (m_pDepthStencilState) { m_pDepthStencilState->Release();   m_pDepthStencilState = nullptr; }
    if (m_pDepthStencilBuffer) { m_pDepthStencilBuffer->Release(); m_pDepthStencilBuffer = nullptr; }
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
    if (m_pImmediateContext) { m_pImmediateContext->Release(); m_pImmediateContext = nullptr; }
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
    // Debug ����� ���� ����� ����̽� �÷��� ����
#ifdef _DEBUG
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // ���� feature level 11.1 ���� ������ ��
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
    pBackBuffer = nullptr;

    // ���� ���� ����ü�� �ʱ�ȭ�մϴ�
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // ���� ���� ����ü�� �ۼ��մϴ�
    depthBufferDesc.Width = data.width;
    depthBufferDesc.Height = data.height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // ������ ���̹��� ����ü�� ����Ͽ� ���� ���� �ؽ��ĸ� �����մϴ�
    if (FAILED(GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer)))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create DepthStencil Buffer");
        return false;
    }

    // ���ٽ� ���� ����ü�� �ʱ�ȭ�մϴ�
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // ���ٽ� ���� ����ü�� �ۼ��մϴ�
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // �ȼ� ������ ���ٽ� �����Դϴ�
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // �ȼ� �޸��� ���ٽ� �����Դϴ�
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // ���� ���ٽ� ���¸� �����մϴ�
    if (FAILED(GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create DepthStencil State");
        return false;
    }

    // ���� ���ٽ� ���¸� �����մϴ�
    GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState, 1);

    // ���� ���ٽ� ���� ����ü�� �ʱ�ȭ�մϴ�
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // ���� ���ٽ� �� ����ü�� �����մϴ�
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // ���� ���ٽ� �並 �����մϴ�
    if (FAILED(GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView)))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create DepthStencil View");
        return false;
    }

    // ������ ��� ��� ���� ���ٽ� ���۸� ��� ���� ������ ���ο� ���ε��մϴ�
    GetDeviceContext()->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // �׷����� ������� ����� ������ ������ ����ü�� �����մϴ�
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // ������ ����ü���� ������ ������ ���¸� ����ϴ�
    if (FAILED(GetDevice()->CreateRasterizerState(&rasterDesc, &m_pRasterState)))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create Raster State");
        return false;
    }

    // ���� ������ ������ ���¸� �����մϴ�
    GetDeviceContext()->RSSetState(m_pRasterState);

    // �������� ���� ����Ʈ�� �����մϴ�
    D3D11_VIEWPORT viewport;
    viewport.Width = data.width;
    viewport.Height = data.height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // ����Ʈ�� �����մϴ�
    GetDeviceContext()->RSSetViewports(1, &viewport);

    // ���� ����� �����մϴ�
    float fieldOfView = XM_PI / 4.0f;
    float screenAspect = data.width / data.height;

    // 3D ������������ ���� ����� ����ϴ�
    float screenNear = 1000.0f;
    float screenDepth = 0.1f;
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

    // ���� ����� �׵� ��ķ� �ʱ�ȭ�մϴ�
    m_worldMatrix = XMMatrixIdentity();

    // 2D ������������ ���� ���� ����� ����ϴ�
    m_orthoMatrix = XMMatrixOrthographicLH(data.width, data.height, screenNear, screenDepth);

    // ���� 2D ������������ Z ���۸� ���� �� ��° ���� ���ٽ� ���¸� ����ϴ�. ������ ��������
    // DepthEnable�� false�� �����ϸ� �ٸ� ��� �Ű� ������ �ٸ� ���� ���ٽ� ���¿� �����մϴ�.
    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
    ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

    depthDisabledStencilDesc.DepthEnable = false;
    depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDisabledStencilDesc.StencilEnable = true;
    depthDisabledStencilDesc.StencilReadMask = 0xFF;
    depthDisabledStencilDesc.StencilWriteMask = 0xFF;
    depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // ��ġ�� ����Ͽ� ���¸� ����ϴ�.
    if (FAILED(GetDevice()->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState)))
    {
        KEPLER_CORE_ASSERT(false, "Fail to Create DepthDisabledStencil State");
        return false;
    }

    return true;
}

void kepler::DX11Context::SwapBuffer()
{
    m_pSwapChain->Present((m_bVSync ? 1 : 0), 0);
}

void kepler::DX11Context::TurnZBufferOn()
{
    GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

void kepler::DX11Context::TurnZBufferOff()
{
    GetDeviceContext()->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
}