#pragma once

#include <DirectXMath.h>

#include "Core/Base.h"

namespace kepler {
    using namespace DirectX;
    
    class DX11TextureShader
    {
    private:
        struct MatrixBufferType
        {
            XMMATRIX world;
            XMMATRIX view;
            XMMATRIX projection;
        };

    public:
        DX11TextureShader();
        DX11TextureShader(const DX11TextureShader& other);
        ~DX11TextureShader();

        bool Init(ID3D11Device* pDevice, HWND hwnd);
        void Shutdown();
        bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMMATRIX worldMatrix,
            XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture);

    private:
        bool InitShader(ID3D11Device* pDevice, HWND hwnd, WCHAR* pVsFilename, WCHAR* pPsFilename);
        void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hwnd, WCHAR* pShaderFilename);

        bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
            XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture);
        void RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount);

    private:
        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
        ID3D11InputLayout* m_pInputLayout = nullptr;
        ID3D11Buffer* m_pMatrixBuffer = nullptr;

        ID3D11SamplerState* m_pSampleState = nullptr;
    };
}