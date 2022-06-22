#pragma once

#include "Renderer/Buffer.h"

namespace kepler {

	class DX11VertexBuffer : public IVertexBuffer
	{
	private:
		ID3D11Buffer*		m_pBuffer;
		BufferLayout		m_layout;

	public:
		DX11VertexBuffer(const uint32_t size, eBufferUsage usage);
		DX11VertexBuffer(const void* const vertices, const uint32_t size, eBufferUsage usage);
		~DX11VertexBuffer();

		// Inherited via IVertexBuffer
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetData(const void* data, uint32_t size) override;
		inline virtual void SetLayout(const BufferLayout& bufferLayout) override;
		inline virtual const BufferLayout& GetLayout() const override { return m_layout; }
	};

	class DX11IndexBuffer : public IIndexBuffer
	{
	private:
		ID3D11Buffer* m_pBuffer;
		uint32_t m_count;

	public:
		DX11IndexBuffer(const uint32_t* const indices, const uint32_t count);
		~DX11IndexBuffer();
		
		// Inherited via IIndexBuffer
		virtual void Bind() override;
		virtual void Unbind() override;
		inline virtual uint32_t GetCount() const override { return m_count; }
	};
}