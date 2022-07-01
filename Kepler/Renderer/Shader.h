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
	// 셰이더 생성(불러오기)과 관리은 모두 ShaderCache를 이용해 진행합니다.
	// ...
	// 쉐이더 프로그램 로딩하기 -----------------------------
	// 가장 먼저 쉐이더 프로그램을 로드합니다. 
	// Kepler Engine의 모든 쉐이더는 ShaderCache 객체가 관리합니다.
	// ShaderCache 객체는 쉐이더의 로드와 가장 마지막으로 Bind된 쉐이더를 캐싱합니다.
	// 
	// 예제에서는 kepler engine이 기본적으로 제공해 주는 쉐이더를 사용하므로, kepler 프로젝트 안의 resource 경로를 지정합니다.
	// 만약 클라이언트 프로그래머가 직접 작성한 쉐이더를 사용하려는 경우, client 프로젝트 안에서 쉐이더가 위치한 별도의 resource 경로를 지정하면 됩니다.
	// NOTE: 모든 쉐이더는 쉐이더 캐시를 통해 로드합니다. 단독적으로 생성하지 않습니다.
	// 또한 반드시 필요한 경우가 아니라면 쉐이더 프로그램의 무결성을 위해 shared_ptr 대신 이름을 저장하는 간접적인 방식으로 쉐이더를 이용합니다. 
	// 만약 쉐이더 프로그램에 대한 직접적인 참조(std::shared_ptr을 이용한 참조)가 필요한 경우 다음과 같이 참조합니다.
	// 
	// 1. 가장 먼저 쉐이더를 로드합니다. 이미 로드한 적 있는 쉐이더라면 다시 불러올 필요가 없습니다.
	// 쉐이더 로드는 ShaderCache::Load()를 이용합니다.
	// ex.
	// ShaderCache::Load(type, path);
	// or
	// ShaderCache::Load(type, name, path);
	//
	// * 쉐이더 로드 시 쉐이더 이름을 지정하지 않으면 불러온 쉐이더의 이름은 자동으로 파일 이름이 됩니다.
	// ex.	VSTest.hlsl				-> VSTest
	//		PSVolumetricShadow.hlsh -> PSVolumetricShadow
	// 
	// 2. ShaderCache::GetShader()를 통해 해당 쉐이더에 대한 shared_ptr를 받아옵니다.
	// std::shared_ptr<IShader> pShader = ShaderCache::GetShader(name);
	//

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

		static void Add(const std::string& name, const std::shared_ptr<IShader>& shader);	// 로드한 쉐이더 프로그램을 table에 추가합니다
	public:

		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& name, const std::string& filepath);

		static bool IsLoaded(const std::string& name);
		static std::shared_ptr<IShader> GetShader(const std::string& name);
		static std::shared_ptr<IShader> GetLastCachedShader(const eShaderType& type);		// 프로그램 타입에 따라 가장 마지막으로 bind된 쉐이더를 참조합니다
		static void SetLastCachedShader(const eShaderType& type, const std::string& name);	// 쉐이더 Bind시 자동으로 호출됩니다. 일반적으로 사용자가 호출하지 않습니다.
	};
	// Shader Interface
	class IShader
	{
	protected:
		friend void ShaderCache::SetLastCachedShader(const eShaderType& type, const std::string& name);

	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		// 쉐이더 내부 변수에 값을 지정할 때 사용
		virtual void SetInt(const std::string& paramName, const int value) = 0;
		virtual void SetFloat(const std::string& paramName, const float value) = 0;
		virtual void SetFloat3(const std::string& paramName, const Vec3f& value) = 0;
		virtual void SetFloat4(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetVector(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetMatrix(const std::string& paramName, const Mat44f& value) = 0;

		virtual void SetName(const std::string& name) = 0;
		virtual std::string GetName() const = 0;
		virtual eShaderType GetType() const = 0;
		virtual uint32_t GetInputElementSlot(const std::string& paramName, const uint32_t paramIndex = 0) const = 0;	// 쉐이더의 Input Layout을 기반으로 InputSlot을 찾아 반환합니다.

		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& name, const std::string& filepath);
	};
}