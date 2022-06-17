#include "kepch.h"

#include "DX11Buffer.h"
#include "DX11Context.h"
#include "Renderer/Shader.h"

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
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        // 별도의 정점데이터가 제공되지 않은 빈 buffer의 경우 언제든지 write될 수 있어야 하므로 dynamic usage로 생성
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

    DX11VertexBuffer::DX11VertexBuffer(const void* const vertices, const uint32_t size, eBufferUsage usage)
        : m_layout{}
        , m_pBuffer{ nullptr }
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

    DX11VertexBuffer::~DX11VertexBuffer()
    {
        if (m_pBuffer)
        {
            m_pBuffer->Release();
            m_pBuffer = nullptr;
        }
    }

    // 렌더링 파이프라인에 바인딩
    void DX11VertexBuffer::Bind()
    {
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

        // VertexBuffer의 레이아웃 내 element들이 어떻게 chunking되었는지에 따라 offset, stride 계산해 쉐이더 장치에 bind
        for (const auto& e : m_layout)
        {
            UINT stride = m_layout.GetStride();
            UINT offset = e.offset;
            uint32_t slot = ShaderCache::GetLastCachedShader(eShaderType::Vertex)->GetInputElementSlot(e.name, e.index);
            
            pDeviceContext->IASetVertexBuffers(slot, 1, &m_pBuffer, &stride, &offset);
        }
    }

    // 렌더링 파이프라인에서 바인딩 해제
    void DX11VertexBuffer::Unbind()
    {
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

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
    DX11IndexBuffer::DX11IndexBuffer(const uint32_t* const indices, const uint32_t count)
        : m_count{count}
        , m_pBuffer{ nullptr }
    {
        ID3D11Device* pDevice;
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(&pDevice, &pDeviceContext);

        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = count * sizeof(uint32_t);
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

    DX11IndexBuffer::~DX11IndexBuffer()
    {
        if (m_pBuffer)
        {
            m_pBuffer->Release();
            m_pBuffer = nullptr;
        }
    }

    // 렌더링 파이프라인에 바인딩
    void DX11IndexBuffer::Bind()
    {
        ID3D11DeviceContext* pDeviceContext;
        GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

        pDeviceContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
        pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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