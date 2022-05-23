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
		
		// ���� Ŀ���� ���� ���̺귯���� ������ �����̱� ������ ������ DXMATH�� ����մϴ�.
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

	// ���ø����̼� �ȿ��� �ε�� ��� ���̴� ���α׷����� �����մϴ�.
	// ���̴� ������ ��� ShaderCache�� �̿��� �����մϴ�.
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