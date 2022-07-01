#pragma once

#include "Renderer/Shader.h"

namespace kepler {

	class DX11Shader : public IShader
	{
	private:
		union
		{
			ID3D11VertexShader*		m_pVertexShader;
			ID3D11GeometryShader*	m_pGeometryShader;
			ID3D11PixelShader*		m_pPixelShader;
			ID3D11HullShader*		m_pHullShader;
			ID3D11DomainShader*		m_pDomainShader;
			ID3D11ComputeShader*	m_pComputeShader;
			void*					m_pVoidShader;
		};
		ID3D11ShaderReflection*		m_pReflection;
		ID3D11InputLayout*			m_pVertexLayout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElemDescs;

		ID3D11Buffer**				m_ppConstantBuffers;
		uint32_t					m_constantBufferCount;	// 쉐이더에 존재하는 constant buffer의 개수
		std::vector<uint32_t>		m_constantBufferSize;	// constant buffer별 크기
		std::vector<char*>			m_pByteBuffer;			// constant buffer와 매핑된 byte 메모리

		std::string			m_name;
		const eShaderType	m_type;

		void Init(const std::string& filepath);
		void Compile(ID3DBlob** ppOutBlob, const std::string& filepath, const std::string& entryPointName = "main");
		void Create(ID3DBlob* pInBlob );
		void InitReflection(ID3DBlob* pInBlob);
		void InitVertexLayout(ID3DBlob* pInBlob);
		void InitConstantBuffer();
		
		bool GetConstantBufferDataInfo(const std::string& inParamName, int& outIndex, int& outOffset);
		void UpdateConstantBuffer(const int index);

	public:
		DX11Shader(const eShaderType& type, const std::string& filepath);
		DX11Shader(const eShaderType& type, const std::string& name, const std::string& filepath);

		virtual ~DX11Shader() override;
		// Inherited via IShader
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetInt(const std::string& paramName, const int value) override;
		virtual void SetFloat(const std::string& paramName, const float value) override;
		virtual void SetFloat3(const std::string& paramName, const Vec3f& value) override;
		virtual void SetFloat4(const std::string& paramName, const Vec4f& value) override;
		virtual void SetVector(const std::string& paramName, const Vec4f& value) override;
		virtual void SetMatrix(const std::string& paramName, const Mat44f& value) override;

		inline virtual void SetName(const std::string& name) override { m_name = name; }
		inline virtual std::string GetName() const override { return m_name; }
		inline virtual eShaderType GetType() const override { return m_type; }
		virtual uint32_t GetInputElementSlot(const std::string& paramName, const uint32_t paramIndex) const override;
	};
}