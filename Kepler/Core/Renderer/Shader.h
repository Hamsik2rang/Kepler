#pragma once

#include <unordered_map>
#include <string>
#include "Core/Base.h"
#include "KeplerMath.h"

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

	// ���ø����̼� �ȿ��� �ε�� ��� ���̴� ���α׷����� �����մϴ�.
	// ���̴� ����(�ҷ�����)�� ������ ��� ShaderCache�� �̿��� �����մϴ�.
	// ...
	// ���̴� ���α׷� �ε��ϱ� -----------------------------
	// ���� ���� ���̴� ���α׷��� �ε��մϴ�. 
	// Kepler Engine�� ��� ���̴��� ShaderCache ��ü�� �����մϴ�.
	// ShaderCache ��ü�� ���̴��� �ε�� ���� ���������� Bind�� ���̴��� ĳ���մϴ�.
	// 
	// ���������� kepler engine�� �⺻������ ������ �ִ� ���̴��� ����ϹǷ�, kepler ������Ʈ ���� resource ��θ� �����մϴ�.
	// ���� Ŭ���̾�Ʈ ���α׷��Ӱ� ���� �ۼ��� ���̴��� ����Ϸ��� ���, client ������Ʈ �ȿ��� ���̴��� ��ġ�� ������ resource ��θ� �����ϸ� �˴ϴ�.
	// NOTE: ��� ���̴��� ���̴� ĳ�ø� ���� �ε��մϴ�. �ܵ������� �������� �ʽ��ϴ�.
	// ���� �ݵ�� �ʿ��� ��찡 �ƴ϶�� ���̴� ���α׷��� ���Ἲ�� ���� shared_ptr ��� �̸��� �����ϴ� �������� ������� ���̴��� �̿��մϴ�. 
	// ���� ���̴� ���α׷��� ���� �������� ����(std::shared_ptr�� �̿��� ����)�� �ʿ��� ��� ������ ���� �����մϴ�.
	// 
	// 1. ���� ���� ���̴��� �ε��մϴ�. �̹� �ε��� �� �ִ� ���̴���� �ٽ� �ҷ��� �ʿ䰡 �����ϴ�.
	// ���̴� �ε�� ShaderCache::Load()�� �̿��մϴ�.
	// ex.
	// ShaderCache::Load(type, path);
	// or
	// ShaderCache::Load(type, name, path);
	//
	// * ���̴� �ε� �� ���̴� �̸��� �������� ������ �ҷ��� ���̴��� �̸��� �ڵ����� ���� �̸��� �˴ϴ�.
	// ex.	VSTest.hlsl				-> VSTest
	//		PSVolumetricShadow.hlsh -> PSVolumetricShadow
	// 
	// 2. ShaderCache::GetShader()�� ���� �ش� ���̴��� ���� shared_ptr�� �޾ƿɴϴ�.
	// std::shared_ptr<IShader> pShader = ShaderCache::GetShader(name);
	//

	class ShaderCache
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<IShader>> s_shaderTable;

		static void Add(const std::string& name, const std::shared_ptr<IShader>& shader);	// �ε��� ���̴� ���α׷��� table�� �߰��մϴ�
	public:

		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Load(const eShaderType& type, const std::string& name, const std::string& filepath);

		static bool IsLoaded(const std::string& name);
		static std::shared_ptr<IShader> GetShader(const std::string& name);
		static std::shared_ptr<IShader> GetLastCachedShader(const eShaderType& type);		// ���α׷� Ÿ�Կ� ���� ���� ���������� bind�� ���̴��� �����մϴ�
		static void SetLastCachedShader(const eShaderType& type, const std::string& name);	// ���̴� Bind�� �ڵ����� ȣ��˴ϴ�. �Ϲ������� ����ڰ� ȣ������ �ʽ��ϴ�.
	};
	// Shader Interface
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		// ���̴� ���� ������ ���� ������ �� ���
		virtual void SetInt(const std::string& paramName, const int value) = 0;
		virtual void SetFloat(const std::string& paramName, const float value) = 0;
		virtual void SetFloat3(const std::string& paramName, const Vec3f& value) = 0;
		virtual void SetFloat4(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetVector(const std::string& paramName, const Vec4f& value) = 0;
		virtual void SetMatrix(const std::string& paramName, const Mat44f& value) = 0;
		virtual void SetArray(const std::string& paramName, const void* pValue, const size_t size) = 0;

		virtual void SetName(const std::string& name) = 0;
		virtual std::string GetName() const = 0;
		virtual eShaderType GetType() const = 0;
		virtual uint32_t GetInputElementSlot(const std::string& paramName, const uint32_t paramIndex = 0) const = 0;	// ���̴��� Input Layout�� ������� InputSlot�� ã�� ��ȯ�մϴ�.

		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& filepath);
		static std::shared_ptr<IShader> Create(const eShaderType& type, const std::string& name, const std::string& filepath);
	};
}