#pragma once

#include "Core/Base.h"

namespace kepler {

	enum class ETextureDataType
	{
		None = 0,
		Float_RGBA16,
		Float_RGB32,
		Float_RGBA32,
		UNorm_RGBA8
	};

	// Texture Interface
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind(uint32_t slot) = 0;
		virtual void* GetData() = 0;
		virtual void* GetTexture() = 0;
		virtual ETextureDataType GetType() = 0;
	};

	// Texture1D Interface
	class ITexture1D : public ITexture
	{
	public:
		virtual uint32_t GetWidth() const = 0;

		virtual void Bind(const uint32_t slot = 0) = 0;
		virtual void SetData(const void* pData, const uint32_t size) = 0;
		virtual void* GetData() = 0;
		virtual void* GetTexture() = 0;
		virtual ETextureDataType GetType() = 0;

		static std::shared_ptr<ITexture1D> Create(const ETextureDataType type, const uint32_t width, const uint8_t channel, const uint8_t bytePerTexel);
	};
	// Texture2D Interface
	class ITexture2D : public ITexture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(const uint32_t slot = 0) = 0;
		virtual void SetData(const void* pData, const uint32_t width, const uint32_t height) = 0;
		virtual void* GetData() = 0;
		virtual void* GetTexture()= 0;
		virtual ETextureDataType GetType() = 0;

		static std::shared_ptr<ITexture2D> Create(const ETextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel = 0, const uint8_t bytePerTexel = 4);
		static std::shared_ptr<ITexture2D> Create(const ETextureDataType type, const std::string& filepath);
	};

	// Texture3D Interface
	// Is it worth?
	class ITexture3D : public ITexture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetDepth() const = 0;

		virtual void Bind(const uint32_t slot = 0) = 0;
		virtual void SetData(const void* pData, const uint32_t size) = 0;
		virtual void* GetData() = 0;
		virtual void* GetTexture() = 0;
		virtual ETextureDataType GetType() = 0;

		static std::shared_ptr<ITexture3D> Create(const ETextureDataType type,
			const uint32_t width,
			const uint32_t height,
			const uint32_t depth,
			const uint8_t channel,
			const uint8_t bytePerTexel);
	};
}