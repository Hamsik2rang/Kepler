#pragma once

#include "Core/Base.h"

namespace kepler {

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind() = 0;

		virtual void SetData(const void* data, const uint32_t size) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class ITexture1D : public ITexture
	{
	public:
		static std::shared_ptr<ITexture1D> Create(const uint32_t length);
	};

	class ITexture2D : public ITexture
	{
	public:
		static std::shared_ptr<ITexture2D> Create(const uint32_t width, const uint32_t height);
		static std::shared_ptr<ITexture2D> Create(const std::string& filepath);
	};

	// Is it worth?
	class ITexture3D : public ITexture
	{
	public:
		static std::shared_ptr<ITexture3D> Create(const uint32_t width, const uint32_t height, const uint32_t depth);
	};
}