#pragma once

#include "Core/Base.h"

namespace kepler {

	enum class eTextureDataType
	{
		Typeless = 0, Int, UInt, Float, NormalizedInt, NormalizedUInt,
	};

	// Texture Interface
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind(uint32_t slot) = 0;
		virtual void SetData(const void* data, const uint32_t size) = 0;
	};

	// Texture1D Interface
	class ITexture1D : public ITexture
	{
	public:
		virtual uint32_t GetWidth() const = 0;

		virtual void Bind(const uint32_t slot = 0) = 0;
		virtual void SetData(const void* data, const uint32_t size) = 0;

		static std::shared_ptr<ITexture1D> Create(const eTextureDataType type, const uint32_t width, const uint8_t channel, const uint8_t bytePerTexel);
	};
	// Texture2D Interface
	class ITexture2D : public ITexture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(const uint32_t slot = 0) = 0;
		virtual void SetData(const void* data, const uint32_t size) = 0;

		static std::shared_ptr<ITexture2D> Create(const eTextureDataType type, const uint32_t width, const uint32_t height, const uint8_t channel, const uint8_t bytePerTexel);
		static std::shared_ptr<ITexture2D> Create(const eTextureDataType type, const std::string& filepath);
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
		virtual void SetData(const void* data, const uint32_t size) = 0;

		static std::shared_ptr<ITexture3D> Create(const eTextureDataType type, 
												  const uint32_t width, 
												  const uint32_t height, 
												  const uint32_t depth, 
												  const uint8_t channel, 
												  const uint8_t bytePerTexel);
	};
}