#pragma once

#include <unordered_map>
#include <string>
#include "Core/Base.h"
#include "Math/KeplerMath.h"

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

	class IShader;

	// 어플리케이션 안에서 로드된 모든 셰이더 프로그램들을 관리합니다.
	// 셰이더 생성은 모두 ShaderCache를 이용해 진행합니다.
	class ShaderCache
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<IShader>> s_shaderTable;
		static std::shared_ptr<IShader> s_pLastCachedVertexShader;
		static std::shared_ptr<IShader> s_pLastCachedPixelShader;
		static std::shared_ptr<IShader> s_pLastCachedGeometryShader;
		static std::shared_ptr<IShader> s_pLastCachedHullShader;
		static std::shared_ptr<IShader> s_pLastCachedDomainShader;
		static std::shared_ptr<IShader> s_pLastCachedComputeShader;

		static void Add(const std::string& name, const std::shared_ptr<IShader>& shader);
	public:

		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& name, const std::string& filepath);

		static bool IsLoaded(const std::string& name);
		static std::shared_ptr<IShader> GetShader(const std::string& name);
		static std::shared_ptr<IShader> GetLastCachedShader(const eShaderType& type);
		static void SetLastCachedShader(const eShaderType& type, const std::string& name);
	};

	class IShader
	{
	protected:
		friend void ShaderCache::SetLastCachedShader(const eShaderType& type, const std::string& name);

	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		virtual void SetInt(const std::string& paramName, const int value) = 0;
		virtual void SetFloat(const std::string& paramName, const float value) = 0;
		virtual void SetFloat3(const std::string& paramName, const Vec3f& value) = 0;
		virtual void SetFloat4(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetVector(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetMatrix(const std::string& paramName, const Mat44f& value) = 0;

		virtual void SetName(const std::string& name) = 0;
		virtual std::string GetName() const = 0;
		virtual eShaderType GetType() const = 0;
		virtual void* GetRawProgram() const = 0;
		virtual uint32_t GetInputElementSlot(const std::string& paramName, const uint32_t paramIndex = 0) const = 0;

		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& name, const std::string& filepath);
	};

	
}