#pragma once

#include <unordered_map>
#include <string>
#include "Core/Base.h"
#include "DirectXMath.h"

namespace kepler {

	enum class eShaderType
	{
		None = 0,
		Vertex,
		Geometry,
		Pixel,

		Hull,
		Domain,
		Compute,
	};

	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		// 추후 커스텀 수학 라이브러리로 수정할 예정이기 때문에 지금은 DXMATH로 사용합니다.
		virtual void SetInt(const std::string& paramName, int value) = 0;
		virtual void SetFloat(const std::string& paramName, float value) = 0;
		virtual void SetFloat3(const std::string& paramName, DirectX::XMFLOAT3 value) = 0;
		virtual void SetFloat4(const std::string& paramName, DirectX::XMFLOAT4 value) = 0;
		virtual void SetVector(const std::string& paramName, DirectX::XMVECTOR value) = 0;
		virtual void SetMatrix(const std::string& paramName, DirectX::XMMATRIX value) = 0;

		virtual void SetName(const std::string& name) = 0;
		virtual std::string GetName() const = 0;
		virtual eShaderType GetType() const = 0;
		virtual void* GetRawProgram() const = 0;

		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& name, const std::string& filepath);
	};

	// 어플리케이션 안에서 로드된 모든 셰이더 프로그램들을 관리합니다.
	// 셰이더 생성은 모두 ShaderCache를 이용해 진행합니다.
	class ShaderCache
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<IShader>> s_shaderTable;
		static std::shared_ptr<IShader> s_pCurBoundShader;

	public:
		static void Add(const std::string& name, const std::shared_ptr<IShader>& shader);
		static void Add(const std::shared_ptr<IShader>& shader);

		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& name, const std::string& filepath);

		static bool IsLoaded(const std::string& name);
		static std::shared_ptr<IShader> GetShader(const std::string& name);
		static std::shared_ptr<IShader> GetBoundShader();
	};
}