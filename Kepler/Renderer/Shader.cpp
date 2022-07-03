#include "kepch.h"

#include "Shader.h"
#include "Renderer/GraphicsAPI.h"
#include "Platform/DirectX11/DX11Shader.h"

namespace kepler {

	//////////// Shader ////////////
	std::shared_ptr<IShader> IShader::Create(const eShaderType& type, const std::string& filepath)
	{
		eGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case eGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11Shader>(type, filepath);
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	std::shared_ptr<IShader> IShader::Create(const eShaderType& type, const std::string& name, const std::string& filepath)
	{
		eGraphicsAPI api = IGraphicsAPI::GetAPI();
		switch (api)
		{
		case eGraphicsAPI::None:
			{
				KEPLER_ASSERT(false, "None Graphics API is not supported");
				return nullptr;
			}
		case eGraphicsAPI::DirectX11:
			{
				return std::make_shared<DX11Shader>(type, name, filepath);
			}
		}

		KEPLER_ASSERT(false, "Invalid Graphics API");
		return nullptr;
	}

	//////////// Shader Cache ////////////
	std::unordered_map<std::string, std::shared_ptr<IShader>> ShaderCache::s_shaderTable;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedVertexShader = nullptr;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedPixelShader = nullptr;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedGeometryShader = nullptr;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedHullShader = nullptr;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedDomainShader = nullptr;
	std::shared_ptr<IShader> ShaderCache::s_pLastCachedComputeShader = nullptr;

	void ShaderCache::Add(const std::string& name, const std::shared_ptr<IShader>& shader)
	{
		s_shaderTable.insert(std::make_pair(name, shader));
	}

	std::shared_ptr<IShader> ShaderCache::Load(const eShaderType& type, const std::string& filepath)
	{
		size_t dirIndex = filepath.find_last_of('/');
		size_t dotIndex = filepath.find_last_of('.');
		std::string name = std::string(filepath.begin() + dirIndex + 1, filepath.begin() + dotIndex);
		std::shared_ptr<IShader> shader = IShader::Create(type, name, filepath);
		Add(name, shader);

		return s_shaderTable[name];
	}

	std::shared_ptr<IShader> ShaderCache::Load(const eShaderType& type, const std::string& name, const std::string& filepath)
	{
		std::shared_ptr<IShader> shader = IShader::Create(type, name, filepath);
		Add(name, shader);

		return s_shaderTable[name];
	}

	bool ShaderCache::IsLoaded(const std::string& name)
	{
		if (s_shaderTable.find(name) != s_shaderTable.end())
		{
			return true;
		}

		return false;
	}

	std::shared_ptr<IShader> ShaderCache::GetShader(const std::string& name)
	{
		if (IsLoaded(name))
		{
			switch (s_shaderTable[name]->GetType())
			{
			case eShaderType::Vertex:	s_pLastCachedVertexShader = s_shaderTable[name]; break;
			case eShaderType::Pixel:	s_pLastCachedPixelShader = s_shaderTable[name]; break;
			case eShaderType::Geometry: s_pLastCachedGeometryShader = s_shaderTable[name]; break;
			case eShaderType::Hull:		s_pLastCachedHullShader = s_shaderTable[name]; break;
			case eShaderType::Domain:	s_pLastCachedDomainShader = s_shaderTable[name]; break;
			case eShaderType::Compute:	s_pLastCachedComputeShader = s_shaderTable[name]; break;
			}

			return s_shaderTable[name];
		}

		KEPLER_ASSERT(false, "Shader not exist");
		return nullptr;
	}

	std::shared_ptr<IShader> ShaderCache::GetLastCachedShader(const eShaderType& type)
	{
		switch (type)
		{
		case eShaderType::Vertex:	return s_pLastCachedVertexShader;
		case eShaderType::Pixel:	return s_pLastCachedPixelShader;
		case eShaderType::Geometry: return s_pLastCachedGeometryShader;
		case eShaderType::Hull:		return s_pLastCachedHullShader;
		case eShaderType::Domain:	return s_pLastCachedDomainShader;
		case eShaderType::Compute:	return s_pLastCachedComputeShader;
		}

		KEPLER_ASSERT(false, "Invalid ShaderType");
		return nullptr;
	}
	void ShaderCache::SetLastCachedShader(const eShaderType& type, const std::string& name)
	{
		switch (type)
		{
		case eShaderType::Vertex:	s_pLastCachedVertexShader = s_shaderTable[name]; break;
		case eShaderType::Pixel:	s_pLastCachedPixelShader = s_shaderTable[name]; break;
		case eShaderType::Geometry: s_pLastCachedGeometryShader = s_shaderTable[name]; break;
		case eShaderType::Hull:		s_pLastCachedHullShader = s_shaderTable[name]; break;
		case eShaderType::Domain:	s_pLastCachedDomainShader = s_shaderTable[name]; break;
		case eShaderType::Compute:	s_pLastCachedComputeShader = s_shaderTable[name]; break;
		}
	}
}