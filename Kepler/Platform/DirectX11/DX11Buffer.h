#pragma once

#include "Core/Renderer/Buffer.h"

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
		inline virtual void SetLayout(const BufferLayout& bufferLayout) override { m_layout = bufferLayout; }
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

	class DX11InstanceBuffer : public IInstanceBuffer
	{
	private:
		ID3D11Buffer*	m_pBuffer;
		uint32_t		m_count;
		BufferLayout	m_layout;

	public:
		DX11InstanceBuffer(const uint32_t size, const uint32_t count, eBufferUsage usage);
		DX11InstanceBuffer(const void* const data, const uint32_t size, const uint32_t count, eBufferUsage usage);
		~DX11InstanceBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(const void* data, uint32_t size, uint32_t count) override;
		inline virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }
		inline virtual const BufferLayout& GetLayout() const override { return m_layout; }
		inline virtual uint32_t GetCount() const override { return m_count; }
	};
}