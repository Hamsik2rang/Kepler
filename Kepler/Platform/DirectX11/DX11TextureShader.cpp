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
        // 정점 및 픽셀 쉐이더를 초기화합니다.
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
        // 렌더링에 사용할 셰이더 매개 변수를 설정합니다.
        if (!SetShaderParameters(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix, pTexture))
        {
            return false;
        }

        // 설정된 버퍼를 셰이더로 렌더링한다.
        RenderShader(pDeviceContext, indexCount);

        return true;
    }

    bool DX11TextureShader::InitShader(ID3D11Device* pDevice, HWND hwnd, WCHAR* pVsFilename, WCHAR* pPsFilename)
    {
        HRESULT result;
        ID3DBlob* errorMessage = nullptr;

        // 버텍스 쉐이더 코드를 컴파일한다.
        ID3DBlob* vertexShaderBuffer = nullptr;
        result = D3DCompileFromFile(pVsFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0,
            0, &vertexShaderBuffer, &errorMessage);
        if (FAILED(result))
        {
            // 셰이더 컴파일 실패시 오류메시지를 출력합니다.
            if (errorMessage)
            {
                //OutputShaderErrorMessage(errorMessage, hwnd, pVsFilename);
                KEPLER_CORE_ASSERT(false, errorMessage->GetBufferPointer());
            }
            // 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우입니다.
            else
            {
                //MessageBox(hwnd, pVsFilename, L"Missing Shader File", MB_OK);
                KEPLER_CORE_ASSERT(false, "Fail to find Shader File");
            }

            return false;
        }

        // 픽셀 쉐이더 코드를 컴파일한다.
        ID3DBlob* pixelShaderBuffer = nullptr;
        result = D3DCompileFromFile(pPsFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0,
            0, &pixelShaderBuffer, &errorMessage);
        if (FAILED(result))
        {
            // 셰이더 컴파일 실패시 오류메시지를 출력합니다.
            if (errorMessage)
            {
                OutputShaderErrorMessage(errorMessage, hwnd, pPsFilename);
            }
            // 컴파일 오류가 아니라면 셰이더 파일을 찾을 수 없는 경우입니다.
            else
            {
                MessageBox(hwnd, pPsFilename, L"Missing Shader File", MB_OK);
            }

            return false;
        }

        // 버퍼로부터 정점 셰이더를 생성한다.
        result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
            &m_pVertexShader);
        if (FAILED(result))
        {
            return false;
        }

        // 버퍼에서 픽셀 쉐이더를 생성합니다.
        result = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
            &m_pPixelShader);
        if (FAILED(result))
        {
            return false;
        }

        // 정점 입력 레이아웃 구조체를 설정합니다.
        // 이 설정은 ModelClass와 셰이더의 VertexType 구조와 일치해야합니다.
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

        // 레이아웃의 요소 수를 가져옵니다.
        UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

        // 정점 입력 레이아웃을 만듭니다.
        result = pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
            vertexShaderBuffer->GetBufferSize(), &m_pInputLayout);
        if (FAILED(result))
        {
            return false;
        }

        // 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제합니다.
        vertexShaderBuffer->Release();
        vertexShaderBuffer = nullptr;

        pixelShaderBuffer->Release();
        pixelShaderBuffer = nullptr;

        // 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
        D3D11_BUFFER_DESC matrixBufferDesc;
        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;
        
        // 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
        result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
        if (FAILED(result))
        {
            return false;
        }

        // 텍스처 샘플러 상태 구조체를 생성 및 설정합니다.
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
        
        // 텍스처 샘플러 상태를 만듭니다.
        result = pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState);
        if (FAILED(result))
        {
            return false;
        }

        return true;
    }

    void DX11TextureShader::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND hwnd, WCHAR* pShaderFilename)
    {
        // 에러 메시지를 출력창에 표시합니다.
        OutputDebugStringA(reinterpret_cast<const char*>(pErrorMessage->GetBufferPointer()));

        // 에러 메세지를 반환합니다.
        pErrorMessage->Release();
        pErrorMessage = 0;

        // 컴파일 에러가 있음을 팝업 메세지로 알려줍니다.
        MessageBox(hwnd, L"Error compiling shader.", pShaderFilename, MB_OK);
    }

    bool DX11TextureShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
        XMMATRIX projectionMatrix, ID3D11ShaderResourceView* pTexture)
    {
        // 행렬을 transpose하여 셰이더에서 사용할 수 있게 합니다
        worldMatrix = XMMatrixTranspose(worldMatrix);
        viewMatrix = XMMatrixTranspose(viewMatrix);
        projectionMatrix = XMMatrixTranspose(projectionMatrix);
        
        // 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
        {
            return false;
        }
        
        // 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
        MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
        
        // 상수 버퍼에 행렬을 복사합니다.
        dataPtr->world = worldMatrix;
        dataPtr->view = viewMatrix;
        dataPtr->projection = projectionMatrix;
        
        // 상수 버퍼의 잠금을 풉니다.
        pDeviceContext->Unmap(m_pMatrixBuffer, 0);
        
        // 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
        unsigned int bufferNumber = 0;
        
        // 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
        pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

        // 픽셀 셰이더에서 셰이더 텍스처 리소스를 설정합니다.
        pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

        return true;
    }

    void DX11TextureShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
    {
        // 정점 입력 레이아웃을 설정합니다.
        pDeviceContext->IASetInputLayout(m_pInputLayout);
        
        // 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정합니다.
        pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
        pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

        // 픽셀 쉐이더에서 샘플러 상태를 설정합니다.
        pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

        // 삼각형을 그립니다.
        pDeviceContext->DrawIndexed(indexCount, 0, 0);
    }
}