#include "kepch.h"

#include "DX11Shader.h"
#include "DX11Context.h"
#include "Utility/StringUtility.h"

namespace kepler {

	DX11Shader::DX11Shader(const eShaderType& type, const std::string& filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_pReflection{ nullptr }
		, m_pVertexLayout{ nullptr }
	{
		Init(filepath);
	}

	DX11Shader::DX11Shader(const eShaderType& type, const std::string& name, const std::string& filepath)
		: m_type{ type }
		, m_pVoidShader{ nullptr }
		, m_name{ name }
		, m_pReflection{ nullptr }
		, m_pVertexLayout{ nullptr }
	{
		Init(filepath);
	}

	DX11Shader::~DX11Shader()
	{
		if (m_pReflection)
		{
			m_pReflection->Release();
			m_pReflection = nullptr;
		}
		if (m_pVertexLayout)
		{
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}
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
		// release constant buffers
		for (uint32_t i = 0; i < m_constantBufferCount; i++)
		{
			if (m_ppConstantBuffers[i])
			{
				m_ppConstantBuffers[i]->Release();
				m_ppConstantBuffers[i] = nullptr;
			}
		}
		// relase constant buffer bytes
		for (auto& p : m_pByteBuffer)
		{
			if (p)
			{
				delete[] p;
				p = nullptr;
			}
		}
	}

	// 1. 쉐이더 컴파일
	// 2. 쉐이더 객체 생성
	// 3. 쉐이더 리플렉션 초기화
	// +. vertex shader인 경우라면 Vertex Input Layout 초기화
	// 4. 쉐이더 안의 상수 버퍼 초기화
	void DX11Shader::Init(const std::string& filepath)
	{
		ID3DBlob* pBlob = nullptr;
		Compile(&pBlob, filepath);
		Create(pBlob);
		InitReflection(pBlob);
		if (m_type == eShaderType::Vertex) 
		{
			InitVertexLayout(pBlob);
		}
		InitConstantBuffer();

		if (pBlob)
		{
			pBlob->Release();
			pBlob = nullptr;
		}
	}

	// HLSL 5.0(DirectX 11_0, 11_1 지원 수준)만을 다룬다고 가정하고 컴파일합니다.
	// TODO: 추후 HLSL Version detection 코드를 작성합시다.
	void DX11Shader::Compile(ID3DBlob** ppOutBlob, const std::string& filepath, const std::string& entryPointName)
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

		HRESULT hr = D3DCompileFromFile(wFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPointName.c_str(), target.c_str(), shaderFlags, 0, ppOutBlob, &pErrorBlob);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to compile Shader");
			if (pErrorBlob)
			{
				std::string errorMsg((const char*)pErrorBlob->GetBufferPointer());
				KEPLER_CORE_ASSERT(false, errorMsg.c_str());
			}
		}
		if (pErrorBlob)
		{
			pErrorBlob->Release();
			pErrorBlob = nullptr;
		}
	}

	// 컴파일된 쉐이더 프로그램을 이용해 쉐이더 객체 생성
	void DX11Shader::Create(ID3DBlob* pInBlob)
	{
		ID3D11Device* pDevice;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);
		HRESULT hr = S_OK;
		switch (m_type)
		{
		case eShaderType::Vertex:
			hr = pDevice->CreateVertexShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pVertexShader);
			break;
		case eShaderType::Geometry:
			hr = pDevice->CreateGeometryShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pGeometryShader);
			break;
		case eShaderType::Pixel:
			hr = pDevice->CreatePixelShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pPixelShader);
			break;
		case eShaderType::Domain:
			hr = pDevice->CreateDomainShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pDomainShader);
			break;
		case eShaderType::Hull:
			hr = pDevice->CreateHullShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pHullShader);
			break;
		case eShaderType::Compute:
			hr = pDevice->CreateComputeShader(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), nullptr, &m_pComputeShader);
			break;
		}

		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to create shader");
		}
	}

	// 쉐이더 리플렉션 초기화
	void DX11Shader::InitReflection(ID3DBlob* pInBlob)
	{
		HRESULT hr = D3DReflect(pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(&m_pReflection));
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Get shader reflection");
			return;
		}
	}

	// 쉐이더 Vertex Layout 초기화
	// 리플렉션을 이용해 자동으로 초기화해줌
	void DX11Shader::InitVertexLayout(ID3DBlob* pInBlob)
	{
		ID3D11Device* pDevice;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);

		D3D11_SHADER_DESC shaderDesc{};
		m_pReflection->GetDesc(&shaderDesc);

		for (uint32_t paramIndex = 0; paramIndex < shaderDesc.InputParameters; paramIndex++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
			m_pReflection->GetInputParameterDesc(paramIndex, &paramDesc);
			
			D3D11_INPUT_ELEMENT_DESC curDesc{};
			curDesc.SemanticName = paramDesc.SemanticName;
			curDesc.SemanticIndex = paramDesc.SemanticIndex;
			curDesc.InputSlot = paramIndex;
			curDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			curDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			curDesc.InstanceDataStepRate = 0;

			// 0b0001
			if (paramDesc.Mask == 1)		
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) curDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) curDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) curDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			//0b0011
			else if (paramDesc.Mask <= 3)	
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) curDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) curDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) curDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			//0b0111
			else if (paramDesc.Mask <= 7)	
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) curDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) curDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) curDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			//0b1111
			else if (paramDesc.Mask <= 15)	
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) curDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) curDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) curDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			
			m_inputElemDescs.push_back(curDesc);
		}

		HRESULT hr = pDevice->CreateInputLayout(&m_inputElemDescs[0], static_cast<UINT>(m_inputElemDescs.size()), pInBlob->GetBufferPointer(), pInBlob->GetBufferSize(), &m_pVertexLayout);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to Create Input Layout");
		}
	}

	//쉐이더 리플렉션을 이용해 쉐이더 내 ConstantBuffer 정보 자동으로 불러오는 함수
	void DX11Shader::InitConstantBuffer()
	{
		ID3D11Device* pDevice = nullptr;
		GetDX11DeviceAndDeviceContext(&pDevice, nullptr);

		// 쉐이더 디스크립션 받아오기
		D3D11_SHADER_DESC shaderDesc{};
		HRESULT hr = m_pReflection->GetDesc(&shaderDesc);
		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to get Shader description");
			return;
		}

		// 쉐이더 안의 상수 버퍼 수를 알아냄
		m_constantBufferCount = shaderDesc.ConstantBuffers;
		// 상수 버퍼 개수만큼 상수 버퍼 및 디스크립터 생성
		std::vector<D3D11_BUFFER_DESC> bufferDescs(m_constantBufferCount, D3D11_BUFFER_DESC{});
		m_ppConstantBuffers = new ID3D11Buffer*[m_constantBufferCount];
		for (uint32_t cBufIndex = 0; cBufIndex < m_constantBufferCount; cBufIndex++)
		{
			D3D11_BUFFER_DESC& curBufferDesc = bufferDescs[cBufIndex];
			
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
			m_constantBufferSize.push_back(cBufferDesc.Size);

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
				HRESULT hr = pVariableReflection->GetDesc(&varDesc);
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
			curBufferDesc.ByteWidth = cBufferDesc.Size;
			// 초기화 데이터 할당
			D3D11_SUBRESOURCE_DATA initData{};
			initData.pSysMem = buffer;
			// 버퍼 생성
			HRESULT hr = pDevice->CreateBuffer(&curBufferDesc, &initData, &m_ppConstantBuffers[cBufIndex]);
			if (FAILED(hr))
			{
				KEPLER_CORE_ASSERT(false, "Fail to Create ConstantBuffer");
				delete[] buffer;
				return;
			}
			// constantbuffer와 매핑된 cpu byte memory 저장
			m_pByteBuffer.push_back(std::move(buffer));
		}
	}

	void DX11Shader::Bind()
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);
		switch (m_type)
		{
		case eShaderType::Vertex:	pDeviceContext->IASetInputLayout(m_pVertexLayout); 
									pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);	break;
		case eShaderType::Geometry: pDeviceContext->GSSetShader(m_pGeometryShader, nullptr, 0);	break;
		case eShaderType::Pixel:	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);	break;
		case eShaderType::Domain:	pDeviceContext->DSSetShader(m_pDomainShader, nullptr, 0);	break;
		case eShaderType::Hull:		pDeviceContext->HSSetShader(m_pHullShader, nullptr, 0);		break;
		case eShaderType::Compute:	pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);	break;
		}

		ShaderCache::SetLastCachedShader(m_type, m_name);
	}
	
	// DirectX11의 경우 필수적으로 호출할 필요 없음
	void DX11Shader::Unbind()
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);
		switch (m_type)
		{
		case eShaderType::Vertex:	pDeviceContext->IASetInputLayout(nullptr); 
									pDeviceContext->VSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Geometry: pDeviceContext->GSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Pixel:	pDeviceContext->PSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Domain:	pDeviceContext->DSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Hull:		pDeviceContext->HSSetShader(nullptr, nullptr, 0);	break;
		case eShaderType::Compute:	pDeviceContext->CSSetShader(nullptr, nullptr, 0);	break;
		}
	}

	// 파라미터 이름을 기반으로 해당 파라미터가 존재하는 constant buffer의 index와 constantbuffer 메모리의 시작 지점으로부터의 offset 반환하는 함수 
	bool DX11Shader::GetConstantBufferDataInfo(const std::string& inParamName, int& outIndex, int& outOffset)
	{
		for (uint32_t index = 0; index < m_constantBufferCount; index++)
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

	// constant buffer 갱신 함수
	void DX11Shader::UpdateConstantBuffer(const int index)
	{
		ID3D11DeviceContext* pDeviceContext{ nullptr };
		GetDX11DeviceAndDeviceContext(nullptr, &pDeviceContext);

		// If constantbuffer isn't initialized dynamic usage
		//pDeviceContext->UpdateSubresource(m_ppConstantBuffers[index], 0, nullptr, m_pBufferBytes[index], 0, 0);

		D3D11_MAPPED_SUBRESOURCE ms{};
		HRESULT hr = pDeviceContext->Map(m_ppConstantBuffers[index], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

		if (FAILED(hr))
		{
			KEPLER_CORE_ASSERT(false, "Fail to get mapped system memory to set update constant buffer");
			return;
		}
		char* pMappedMem = (char*)ms.pData;
		::memcpy(pMappedMem, m_pByteBuffer[index], m_constantBufferSize[index]);
		pDeviceContext->Unmap(m_ppConstantBuffers[index], 0);

		switch (m_type)
		{
		case eShaderType::Vertex:	pDeviceContext->VSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		case eShaderType::Geometry: pDeviceContext->GSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		case eShaderType::Pixel:	pDeviceContext->PSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		case eShaderType::Domain:	pDeviceContext->DSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		case eShaderType::Hull:		pDeviceContext->HSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		case eShaderType::Compute:	pDeviceContext->CSSetConstantBuffers(index, 1, &m_ppConstantBuffers[index]);	break;
		}
	}

	void DX11Shader::SetInt(const std::string& paramName, int value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat(const std::string& paramName, float value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat3(const std::string& paramName, const Vec3f& value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetFloat4(const std::string& paramName, const Vec4f& value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetVector(const std::string& paramName, const Vec4f& value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	void DX11Shader::SetMatrix(const std::string& paramName, const Mat44f& value)
	{
		int index = 0;
		int offset = 0;
		// 쉐이더 변수 이름을 이용해 매핑된 memory의 인덱스와 offset을 찾음
		GetConstantBufferDataInfo(paramName, index, offset);
		if (index < 0)
		{
			KEPLER_WARNING("Invalid shader parameter");
			return;
		}
		// 해당 메모리에 값을 쓴 후 constant buffer update
		memcpy(m_pByteBuffer[index] + offset, &value, sizeof(value));
		UpdateConstantBuffer(index);
	}

	uint32_t DX11Shader::GetInputElementSlot(const std::string& paramName, const uint32_t paramIndex) const
	{
		uint32_t inputSlot = 0xffffffff;	// inputslot을 찾았는지 구분하기 위한 임의의 maximum value
		for (const auto& e : m_inputElemDescs)
		{
			if (e.SemanticName == paramName && e.SemanticIndex == paramIndex)
			{
				inputSlot = e.InputSlot;
				break;
			}
		}

		// 유효 슬롯 범위를 넘어간 경우(찾지 못한 경우) assertion
		KEPLER_ASSERT(inputSlot < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Fail to get Input Element Slot");

		return inputSlot;
	}
}