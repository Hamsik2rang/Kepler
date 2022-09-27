#include "kepch.h"

#include "DX11Buffer.h"
#include "DX11Context.h"
#include "Core/Renderer/Shader.h"
#include "DX11State.h"

namespace kepler {

	// �߻�ȭ�� ���̴� ������ Ÿ���� DirectX11 ���̴� ������ Ÿ������ �����ϴ� �Լ�
	static DXGI_FORMAT ShaderDataTypeToDXGIFormat(EShaderDataType type)
	{
		switch (type)
		{
		case EShaderDataType::Bool:     return DXGI_FORMAT_R8_UINT;
		case EShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case EShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case EShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case EShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case EShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
		case EShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
		case EShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
		case EShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
		case EShaderDataType::Float33:  return DXGI_FORMAT_R32G32B32_FLOAT;
		case EShaderDataType::Float44:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		KEPLER_ASSERT(false, "Invalid ShaderDataType");
		return DXGI_FORMAT_UNKNOWN;
	}

	/////////// VertexBuffer Member Functions ///////////
	DX11VertexBuffer::DX11VertexBuffer(const uint32_t size, EBufferUsage usage)
		: m_layout{}
		, m_pBuffer{ nullptr }
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		// ������ ���������Ͱ� �������� ���� �� buffer�� ��� �������� write�� �� �־�� �ϹǷ� dynamic usage�� ����
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = nullptr;
		HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
		if (FAILED(hr))
		{
			KEPLER_ASSERT_WITH_ERROR_CODE(false);
		}
	}

	DX11VertexBuffer::DX11VertexBuffer(const void* const vertices, const uint32_t size, EBufferUsage usage)
		: m_layout{}
		, m_pBuffer{ nullptr }
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = 0;
		
		switch (usage)
		{
		case EBufferUsage::Default:
		case EBufferUsage::Static:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			break;
		case EBufferUsage::Immutable:
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.CPUAccessFlags = 0;
			break;
		case EBufferUsage::Dynamic:
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case EBufferUsage::Staging:
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

	// ������ ���������ο� ���ε�
	void DX11VertexBuffer::Bind()
	{
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();
		auto& desc = IRenderState::Get()->GetShaderState();

		// VertexBuffer�� ���̾ƿ� �� element���� ��� chunking�Ǿ������� ���� offset, stride ����� ���̴� ��ġ�� bind
		for (const auto& e : m_layout)
		{
			UINT stride = m_layout.GetStride();
			UINT offset = e.offset;
			uint32_t slot = desc.pVertexShader->GetInputElementSlot(e.name, e.index);

			pDeviceContext->IASetVertexBuffers(slot, 1, &m_pBuffer, &stride, &offset);
		}
	}

	// ������ ���������ο��� ���ε� ����
	void DX11VertexBuffer::Unbind()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();

		auto& desc = IRenderState::Get()->GetShaderState();

		for (const auto& e : m_layout)
		{
			UINT stride = m_layout.GetStride();
			UINT offset = e.offset;
			uint32_t slot = desc.pVertexShader->GetInputElementSlot(e.name, e.index);

			pContext->IASetVertexBuffers(slot, 1, nullptr, nullptr, nullptr);
		}
	}

	// VertexBuffer�� ���� ������ ����
	void DX11VertexBuffer::SetData(const void* data, uint32_t size)
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		// TODO: ���� ���� ����
	}

	/////////// IndexBuffer Member Functions ///////////
	DX11IndexBuffer::DX11IndexBuffer(const uint32_t* const indices, const uint32_t count)
		: m_count{ count }
		, m_pBuffer{ nullptr }
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = count * sizeof(uint32_t);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;
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

	// ������ ���������ο� ���ε�
	void DX11IndexBuffer::Bind()
	{
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		pDeviceContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// ������ ���������ο��� ���ε� ����
	void DX11IndexBuffer::Unbind()
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	/////////// InstanceBuffer Member Functions ///////////
	DX11InstanceBuffer::DX11InstanceBuffer(const uint32_t size, const uint32_t count, const EBufferUsage usage)
		: m_layout{}
		, m_pBuffer{ nullptr }
		, m_count{ count }
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		// ������ �ν��Ͻ� �����Ͱ� �������� ���� �� buffer�� ��� �������� write�� �� �־�� �ϹǷ� dynamic usage�� ����
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = nullptr;
		HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
		if (FAILED(hr))
		{
			KEPLER_ASSERT_WITH_ERROR_CODE(false);
		}
	}

	DX11InstanceBuffer::DX11InstanceBuffer(const void* const data, const uint32_t size, const uint32_t count, const EBufferUsage usage)
		: m_layout{}
		, m_pBuffer{ nullptr }
		, m_count{ count }
	{
		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.CPUAccessFlags = 0;

		switch (usage)
		{
		case EBufferUsage::Default:
		case EBufferUsage::Static:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
			break;
		case EBufferUsage::Immutable:
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.CPUAccessFlags = 0;
			break;
		case EBufferUsage::Dynamic:
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case EBufferUsage::Staging:
			bufferDesc.Usage = D3D11_USAGE_STAGING;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			break;
		}

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = data;
		HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &initData, &m_pBuffer);
		if (FAILED(hr))
		{
			KEPLER_ASSERT_WITH_ERROR_CODE(false);
		}
	}

	DX11InstanceBuffer::~DX11InstanceBuffer()
	{
		if (m_pBuffer)
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}
	}

	void DX11InstanceBuffer::Bind()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();

		auto& shaderState = IRenderState::Get()->GetShaderState();
		auto& pVertexShader = shaderState.pVertexShader;

		for (const auto& e : m_layout)
		{
			UINT stride = m_layout.GetStride();
			UINT offset = e.offset;
			uint32_t slot = pVertexShader->GetInputElementSlot(e.name, e.index);

			pContext->IASetVertexBuffers(slot, 1, &m_pBuffer, &stride, &offset);
		}
	}

	void DX11InstanceBuffer::Unbind()
	{
		ID3D11DeviceContext* pContext = IGraphicsContext::Get()->GetDeviceContext();

		auto& shaderState = IRenderState::Get()->GetShaderState();
		auto& pVertexShader = shaderState.pVertexShader;

		for (const auto& e : m_layout)
		{
			UINT stride = m_layout.GetStride();
			UINT offset = e.offset;
			uint32_t slot = pVertexShader->GetInputElementSlot(e.name, e.index);

			pContext->IASetVertexBuffers(slot, 1, nullptr, nullptr, nullptr);
		}
	}

	void DX11InstanceBuffer::SetData(const void* data, uint32_t size, uint32_t count)
	{
		//TODO: ���� ���� - ���� ���� ������ ���� �� �����
	}
}