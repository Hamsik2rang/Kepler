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
		m_pBlob->Release();
		m_pBlob = nullptr;
	}

	DX11Shader::DX11Shader(const eShaderType& type, const std::string& filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_pBlob{ nullptr }
	{
		Compile(filepath);
	}

	DX11Shader::DX11Shader(const eShaderType & type, const std::string & name, const std::string & filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_pBlob{ nullptr }
	{
		Compile(filepath);
	}

	DX11Shader::~DX11Shader()
	{
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

	void DX11Shader::SetInt(const std::string & paramName, int value)
	{
		
	}

	void DX11Shader::SetFloat(const std::string & paramName, float value)
	{
	
	}

	void DX11Shader::SetFloat3(const std::string & paramName, DirectX::XMFLOAT3 value)
	{
	
	}

	void DX11Shader::SetFloat4(const std::string & paramName, DirectX::XMFLOAT4 value)
	{
	
	}

	void DX11Shader::SetVector(const std::string & paramName, DirectX::XMVECTOR value)
	{
	
	}

	void DX11Shader::SetMatrix(const std::string & paramName, DirectX::XMMATRIX value)
	{
	
	}
}