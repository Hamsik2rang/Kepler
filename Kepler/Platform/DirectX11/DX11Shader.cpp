#include "kepch.h"

#include "DX11Shader.h"
#include "DX11Context.h"
#include "Utility/StringUtility.h"

namespace kepler {
	// HLSL 5.0(DirectX 11_0, 11_1 지원 수준)만을 다룬다고 가정하고 컴파일합니다.
	// TODO: 추후 HLSL Version detection 코드를 작성합시다.
	void DX11Shader::Compile(const std::string& filepath, const std::string& entryPointName)
	{
		std::wstring wFilePath = utility::StringToWstring(filepath);
		std::string target{ "" };
		switch (m_type)
		{
		case eShaderType::Vertex:
			target = "vs_5_0";
			break;
		case eShaderType::Geometry:
			target = "gs_5_0";
			break;
		case eShaderType::Pixel:
			target = "ps_5_0";
			break;
		case eShaderType::Hull:
			target = "hs_5_0";
			break;
		case eShaderType::Domain:
			target = "ds_5_0";
			break;
		case eShaderType::Compute:
			target = "cs_5_0";
			break;
		default:
			KEPLER_ASSERT(false, "Invalid ShaderType");
		}

		UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob = nullptr;

		HRESULT hr = D3DCompileFromFile(wFilePath.c_str(), nullptr, nullptr, entryPointName.c_str(), target.c_str(), shaderFlags, 0, &m_pBlob, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				std::string errorMsg((const char*)pErrorBlob->GetBufferPointer());
				KEPLER_ASSERT(false, errorMsg.c_str());
			}
		}
		pErrorBlob->Release();
		pErrorBlob = nullptr;
	}

	void DX11Shader::Create()
	{
		ID3D11Device* pDevice;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);
		HRESULT hr = S_OK;
		switch (m_type)
		{
		case eShaderType::Vertex:
			hr = pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pVertexShader);
			break;
		case eShaderType::Geometry:
			hr = pDevice->CreateGeometryShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
			break;
		case eShaderType::Pixel:
			hr = pDevice->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
			break;
		case eShaderType::Domain:
			hr = pDevice->CreateDomainShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pDomainShader);
			break;
		case eShaderType::Hull:
			hr = pDevice->CreateHullShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pHullShader);
			break;
		case eShaderType::Compute:
			hr = pDevice->CreateComputeShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pComputeShader);
			break;
		}

		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create shader");
		}
	}

	void DX11Shader::InitConstantBuffer()
	{
		ID3D11Device* pDevice = nullptr;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);

		HRESULT hr = D3DReflect(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(&m_pReflection));
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Get shader reflection");
			return;
		}

		// 쉐이더 디스크립션 받아오기
		D3D11_SHADER_DESC shaderDesc{};
		if (FAILED(m_pReflection->GetDesc(&shaderDesc)))
		{
			KEPLER_CORE_ASSERT(false, "Fail to get shader reflection data");
			return;
		}
		// 쉐이더 안의 상수 버퍼 수를 알아냄
		m_cBufferCount = shaderDesc.ConstantBuffers;
		// 상수 버퍼 개수만큼 상수 버퍼 및 디스크립터 생성
		std::vector<D3D11_BUFFER_DESC> bufferDescs(m_cBufferCount, D3D11_BUFFER_DESC{});
		m_ppConstantBuffers = new ID3D11Buffer*[m_cBufferCount];
		for (uint32_t cBufIndex = 0; cBufIndex < m_cBufferCount; cBufIndex++)
		{
			D3D11_BUFFER_DESC& curBufferDesc = bufferDescs[cBufIndex];
			// not necessary
			//ZeroMemory(&bufferDescs[cBufIndex], sizeof(D3D11_BUFFER_DESC));

			curBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			curBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			curBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			// 현재 인덱스에 해당하는 상수 버퍼 가져옴
			ID3D11ShaderReflectionConstantBuffer* pConstantBufferReflection = m_pReflection->GetConstantBufferByIndex(cBufIndex);
			if (!pConstantBufferReflection)
			{
				KEPLER_CORE_ASSERT(false, "Fail to get constant buffer reflection data");
				return;
			}

			// 상수 버퍼 디스크립션 받아오기
			D3D11_SHADER_BUFFER_DESC cBufferDesc{};
			if (FAILED(pConstantBufferReflection->GetDesc(&cBufferDesc)))
			{
				KEPLER_CORE_ASSERT(false, "Fail to get constant buffer description");
				return;
			}
			char* buffer = new char[cBufferDesc.Size];
			memset(buffer, 0, cBufferDesc.Size);

			// 각 상수버퍼 변수별로 순회하면서 default value 존재 여부 확인
			uint32_t varCount = cBufferDesc.Variables;
			for (uint32_t varIndex = 0; varIndex < varCount; varIndex++)
			{
				ID3D11ShaderReflectionVariable* pVariableReflection = pConstantBufferReflection->GetVariableByIndex(varIndex);
				if (!pVariableReflection)
				{
					KEPLER_CORE_ASSERT(false, "Fail to get shader variable reflection data");
					delete[] buffer;
					return;
				}
					
				D3D11_SHADER_VARIABLE_DESC varDesc{};
				hr = pVariableReflection->GetDesc(&varDesc);
				if (FAILED(hr))
				{
					KEPLER_CORE_ASSERT(false, "Fail to get variable description");
					delete[] buffer;
					return;
				}

				// default value가 설정되어 있다면 해당 값 채워줌
				if (varDesc.DefaultValue)
				{
					memcpy(buffer + varDesc.StartOffset, varDesc.DefaultValue, varDesc.Size);
				}
			}
			// 상수 버퍼 크기 지정
			curBufferDesc.ByteWidth = sizeof(cBufferDesc.Size);
			// 초기화 데이터 할당
			D3D11_SUBRESOURCE_DATA initData{};
			initData.pSysMem = buffer;
			// 버퍼 생성
			hr = pDevice->CreateBuffer(&curBufferDesc, &initData, &m_ppConstantBuffers[cBufIndex]);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to Create ConstantBuffer");
				delete[] buffer;
				return;
			}

			m_pBufferBytes.push_back(std::move(buffer));
		}
	}

	DX11Shader::DX11Shader(const eShaderType& type, const std::string& filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_pBlob{ nullptr }
	{
		Compile(filepath);
		Create();
		InitConstantBuffer();
	}

	DX11Shader::DX11Shader(const eShaderType& type, const std::string& name, const std::string& filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_pBlob{ nullptr }
	{
		Compile(filepath);
		Create();
		InitConstantBuffer();
	}

	DX11Shader::~DX11Shader()
	{
		// release shader
		if (m_pVertexShader)
		{
			switch (m_type)
			{
			case eShaderType::Vertex:	m_pVertexShader->Release();		break;
			case eShaderType::Geometry: m_pGeometryShader->Release();	break;
			case eShaderType::Pixel:	m_pPixelShader->Release();		break;
			case eShaderType::Domain:	m_pDomainShader->Release();		break;
			case eShaderType::Hull:		m_pHullShader->Release();		break;
			case eShaderType::Compute:	m_pComputeShader->Release();	break;
			}
			m_pVertexShader = nullptr;
		}
		// release raw shader program
		if (m_pBlob)
		{
			m_pBlob->Release();
			m_pBlob = nullptr;
		}
		// release constant buffers
		for (uint32_t i = 0; i < m_cBufferCount; i++)
		{
			if (m_ppConstantBuffers[i])
			{
				m_ppConstantBuffers[i]->Release();
				m_ppConstantBuffers[i] = nullptr;
			}
		}
		// relase constant buffer bytes
		for (auto& p : m_pBufferBytes)
		{
			if (p)
			{
				delete[] p;
				p = nullptr;
			}
		}
	}

	void DX11Shader::Bind()
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);
		switch (m_type)
		{
		case eShaderType::Vertex:	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);	break;
		case eShaderType::Geometry: pDeviceContext->GSSetShader(m_pGeometryShader, nullptr, 0);	break;
		case eShaderType::Pixel:	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);	break;
		case eShaderType::Domain:	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);	break;
		case eShaderType::Hull:		pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);		break;
		case eShaderType::Compute:	pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);	break;
		}
	}

	void DX11Shader::Unbind()
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);
		switch (m_type)
		{
		case eShaderType::Vertex:	pDeviceContext->VSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Geometry: pDeviceContext->GSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Pixel:	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Domain:	pDeviceContext->DSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Hull:		pDeviceContext->HSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Compute:	pDeviceContext->CSSetShader(nullptr, nullptr, 0);	break;
		}
	}

	bool DX11Shader::GetConstantBufferDataInfo(const std::string& inParamName, int& outIndex, int& outOffset)
	{
		for (uint32_t index = 0; index < m_cBufferCount; index++)
		{
			ID3D11ShaderReflectionConstantBuffer* cBufferReflection = m_pReflection->GetConstantBufferByIndex(index);
			if (!cBufferReflection)
			{
				break;
			}

			ID3D11ShaderReflectionVariable* pVariableReflection = cBufferReflection->GetVariableByName(inParamName.c_str());
			if (!pVariableReflection)
			{
				continue;
			}

			D3D11_SHADER_VARIABLE_DESC varDesc{};
			HRESULT hr = pVariableReflection->GetDesc(&varDesc);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to get shader variable reflection data");
				break;
			}
			outIndex = index;
			outOffset = varDesc.StartOffset;
			return true;
		}

		// 찾지 못한 경우
		outIndex = -1;
		outOffset = -1;
		
		return false;
	}

	void DX11Shader::UpdateConstantBuffer(const int index)
	{
		ID3D11DeviceContext* pDeviceContext{ nullptr };
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

		pDeviceContext->UpdateSubresource(m_ppConstantBuffers[index], 0, nullptr, reinterpret_cast<const void*>(m_pBufferBytes[index]), 0, 0);
	}

	void DX11Shader::SetInt(const std::string& paramName, int value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat(const std::string& paramName, float value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat3(const std::string& paramName, DirectX::XMFLOAT3 value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat4(const std::string& paramName, DirectX::XMFLOAT4 value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetVector(const std::string& paramName, DirectX::XMVECTOR value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetMatrix(const std::string& paramName, DirectX::XMMATRIX value)
	{
		int index = 0;
		int offset = 0;
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		memcpy(m_pBufferBytes[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}
}