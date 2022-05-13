#include "kepch.h"

#include "DX11Buffer.h"
#include "DX11Context.h"

namespace kepler {
    // 추상화된 쉐이더 데이터 타입을 DirectX11 쉐이더 데이터 타입으로 변경하는 함수
    static DXGI_FORMAT ShaderDataTypeToDXGIFormat(eShaderDataType type)
    {
        switch (type)
        {
        case eShaderDataType::Bool:     return DXGI_FORMAT_R8_UINT;
        case eShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
        case eShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
        case eShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
        case eShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case eShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
        case eShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
        case eShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
        case eShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
        case eShaderDataType::Float33:  return DXGI_FORMAT_R32G32B32_FLOAT;
        case eShaderDataType::Float44:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        
        KEPLER_ASSERT(false, "Invalid ShaderDataType");
        return DXGI_FORMAT_UNKNOWN;
    }

    /////////// VertexBuffer Member Functions ///////////
    DX11VertexBuffer::DX11VertexBuffer(const uint32_t size, eBufferUsage usage)
		: m_layout{}
		, m_pBuffer{ nullptr }
		, m_pInputLayout{ nullptr }
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        
        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = nullptr;
        HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
        if (FAILED(hr))
        {
            KEPLER_ASSERT_WITH_ERROR_CODE(false);
        }
    }

    DX11VertexBuffer::DX11VertexBuffer(const float* const vertices, const uint32_t size, eBufferUsage usage)
        : m_layout{}
        , m_pBuffer{ nullptr }
		, m_pInputLayout{ nullptr }
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        switch (usage)
        {
		case eBufferUsage::Default:
        case eBufferUsage::Static:
				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.CPUAccessFlags = 0;
                break;
            case eBufferUsage::Immutable: 
                bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                bufferDesc.CPUAccessFlags = 0;
                break;
            case eBufferUsage::Dynamic:
                bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
                bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                break;
            case eBufferUsage::Staging:
                bufferDesc.Usage = D3D11_USAGE_STAGING;
                bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                break;
        }

        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = vertices;
        HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
        if (FAILED(hr))
        {
            KEPLER_ASSERT_WITH_ERROR_CODE(false);
        }
    }

    // 렌더링 파이프라인에 바인딩
    void DX11VertexBuffer::Bind()
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        KEPLER_CORE_ASSERT(m_pInputLayout, "Vertex Buffer has no layout.");

        UINT offset = 0;
        UINT stride = static_cast<UINT>(m_layout.GetStride());
        pDeviceContext->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);
    }

    // 렌더링 파이프라인에서 바인딩 해제
    void DX11VertexBuffer::Unbind()
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

		pDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    }

    // VertexBuffer의 정점 데이터 변경
    void DX11VertexBuffer::SetData(const void* data, uint32_t size)
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        // TODO: 추후 마저 구현
    }

    void DX11VertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_layout = layout;
    }

    /////////// IndexBuffer Member Functions ///////////
    DX11IndexBuffer::DX11IndexBuffer(const uint32_t* const indices, const uint32_t size)
        : m_size{size}
        , m_pBuffer{ nullptr }
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData{};
        initData.pSysMem = indices;
        HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
        if (FAILED(hr))
        {
            KEPLER_ASSERT_WITH_ERROR_CODE(false);
        }
    }

    // 렌더링 파이프라인에 바인딩
    void DX11IndexBuffer::Bind()
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        pDeviceContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
    }

    // 렌더링 파이프라인에서 바인딩 해제
    void DX11IndexBuffer::Unbind()
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
    }
}