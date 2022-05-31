#include "kepch.h"

#include <d3dcompiler.h>
#include "DX11TextureShader.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"

namespace kepler {
    DX11TextureShader::DX11TextureShader()
    {
    }

    DX11TextureShader::DX11TextureShader(const DX11TextureShader& other)
    {
    }

    DX11TextureShader::~DX11TextureShader()
    {
    }

    bool DX11TextureShader::Init(ID3D11Device* device, HWND hwnd)
    {
        // ���� �� �ȼ� ���̴��� �ʱ�ȭ�մϴ�.
        return InitShader(device, hwnd, (WCHAR*)L"../Kepler/Resources/Shaders/HLSL/SimpleVertexShader.hlsl", (WCHAR*)L"../Kepler/Resources/Shaders/HLSL/SimplePixelShader.hlsl");
    }

    void DX11TextureShader::Shutdown()
    {
        if (m_pSampleState) { m_pSampleState->Release(); m_pSampleState = 0; }
        if (m_pMatrixBuffer) { m_pMatrixBuffer->Release();  m_pMatrixBuffer = 0; }
        if (m_pInputLayout) { m_pInputLayout->Release();  m_pInputLayout = 0; }
        if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = 0; }
        if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = 0; }
    }

    bool DX11TextureShader::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMMATRIX worldMatrix,
        XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture)
    {
        // �������� ����� ���̴� �Ű� ������ �����մϴ�.
        if (!SetShaderParameters(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix, pTexture))
        {
            return false;
        }

        // ������ ���۸� ���̴��� �������Ѵ�.
        RenderShader(pDeviceContext, indexCount);

        return true;
    }

    bool DX11TextureShader::InitShader(ID3D11Device* pDevice, HWND hwnd, WCHAR* pVsFilename, WCHAR* pPsFilename)
    {
        HRESULT result;
        ID3DBlob* errorMessage = nullptr;

        // ���ؽ� ���̴� �ڵ带 �������Ѵ�.
        ID3DBlob* vertexShaderBuffer = nullptr;
        result = D3DCompileFromFile(pVsFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0,
            0, &vertexShaderBuffer, &errorMessage);
        if (FAILED(result))
        {
            // ���̴� ������ ���н� �����޽����� ����մϴ�.
            if (errorMessage)
            {
                //OutputShaderErrorMessage(errorMessage, hwnd, pVsFilename);
                KEPLER_CORE_ASSERT(false, errorMessage->GetBufferPointer());
            }
            // ������ ������ �ƴ϶�� ���̴� ������ ã�� �� ���� ����Դϴ�.
            else
            {
                //MessageBox(hwnd, pVsFilename, L"Missing Shader File", MB_OK);
                KEPLER_CORE_ASSERT(false, "Fail to find Shader File");
            }

            return false;
        }

        // �ȼ� ���̴� �ڵ带 �������Ѵ�.
        ID3DBlob* pixelShaderBuffer = nullptr;
        result = D3DCompileFromFile(pPsFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0,
            0, &pixelShaderBuffer, &errorMessage);
        if (FAILED(result))
        {
            // ���̴� ������ ���н� �����޽����� ����մϴ�.
            if (errorMessage)
            {
                OutputShaderErrorMessage(errorMessage, hwnd, pPsFilename);
            }
            // ������ ������ �ƴ϶�� ���̴� ������ ã�� �� ���� ����Դϴ�.
            else
            {
                MessageBox(hwnd, pPsFilename, L"Missing Shader File", MB_OK);
            }

            return false;
        }

        // ���۷κ��� ���� ���̴��� �����Ѵ�.
        result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
            &m_pVertexShader);
        if (FAILED(result))
        {
            return false;
        }

        // ���ۿ��� �ȼ� ���̴��� �����մϴ�.
        result = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
            &m_pPixelShader);
        if (FAILED(result))
        {
            return false;
        }

        // ���� �Է� ���̾ƿ� ����ü�� �����մϴ�.
        // �� ������ ModelClass�� ���̴��� VertexType ������ ��ġ�ؾ��մϴ�.
        D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
        polygonLayout[0].SemanticName = "POSITION";
        polygonLayout[0].SemanticIndex = 0;
        polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        polygonLayout[0].InputSlot = 0;
        polygonLayout[0].AlignedByteOffset = 0;
        polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[0].InstanceDataStepRate = 0;

        polygonLayout[1].SemanticName = "TEXCOORD";
        polygonLayout[1].SemanticIndex = 0;
        polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        polygonLayout[1].InputSlot = 0;
        polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[1].InstanceDataStepRate = 0;

        // ���̾ƿ��� ��� ���� �����ɴϴ�.
        UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

        // ���� �Է� ���̾ƿ��� ����ϴ�.
        result = pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
        if (FAILED(result))
        {
            return false;
        }

        // �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� �����մϴ�.
        vertexShaderBuffer->Release();
        vertexShaderBuffer = nullptr;

        pixelShaderBuffer->Release();
        pixelShaderBuffer = nullptr;

        // ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ��մϴ�.
        D3D11_BUFFER_DESC matrixBufferDesc;
        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;
        
        // ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�.
        result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
        if (FAILED(result))
        {
            return false;
        }

        // �ؽ�ó ���÷� ���� ����ü�� ���� �� �����մϴ�.
        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        
        // �ؽ�ó ���÷� ���¸� ����ϴ�.
        result = pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState);
        if (FAILED(result))
        {
            return false;
        }

        return true;
    }

    void DX11TextureShader::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hwnd, WCHAR* pShaderFilename)
    {
        // ���� �޽����� ���â�� ǥ���մϴ�.
        OutputDebugStringA(reinterpret_cast<const char*>(pErrorMessage->GetBufferPointer()));

        // ���� �޼����� ��ȯ�մϴ�.
        pErrorMessage->Release();
        pErrorMessage = 0;

        // ������ ������ ������ �˾� �޼����� �˷��ݴϴ�.
        MessageBox(hwnd, L"Error compiling shader.", pShaderFilename, MB_OK);
    }

    bool DX11TextureShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
        XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture)
    {
        // ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �մϴ�
        worldMatrix = XMMatrixTranspose(worldMatrix);
        viewMatrix = XMMatrixTranspose(viewMatrix);
        projectionMatrix = XMMatrixTranspose(projectionMatrix);
        
        // ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
        {
            return false;
        }
        
        // ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
        MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
        
        // ��� ���ۿ� ����� �����մϴ�.
        dataPtr->world = worldMatrix;
        dataPtr->view = viewMatrix;
        dataPtr->projection = projectionMatrix;
        
        // ��� ������ ����� Ǳ�ϴ�.
        pDeviceContext->Unmap(m_pMatrixBuffer, 0);
        
        // ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
        unsigned int bufferNumber = 0;
        
        // ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
        pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

        // �ȼ� ���̴����� ���̴� �ؽ�ó ���ҽ��� �����մϴ�.
        pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

        return true;
    }

    void DX11TextureShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
    {
        // ���� �Է� ���̾ƿ��� �����մϴ�.
        pDeviceContext->IASetInputLayout(m_pInputLayout);
        
        // �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� �����մϴ�.
        pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
        pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

        // �ȼ� ���̴����� ���÷� ���¸� �����մϴ�.
        pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

        // �ﰢ���� �׸��ϴ�.
        pDeviceContext->DrawIndexed(indexCount, 0, 0);
    }
}