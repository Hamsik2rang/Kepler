#pragma once

#include "Renderer/Buffer.h"

namespace kepler {

	class DX11VertexBuffer : public IVertexBuffer
	{
	private:
		InputLayout			m_layout;
		ID3D11Buffer*		m_pBuffer;

	public:
		DX11VertexBuffer(uint32_t size);
		DX11VertexBuffer(float* vertices, uint32_t size);
		
		// Inherited via IVertexBuffer
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void SetData(const void* data, uint32_t size) override;
		inline virtual void SetLayout(const InputLayout& layout) override { m_layout = layout; }
		inline virtual const InputLayout& GetLayout() const override { return m_layout; }
	};

	class DX11IndexBuffer : public IIndexBuffer
	{
	private:
		ID3D11Buffer* m_pBuffer;
		uint32_t m_size;

	public:
		DX11IndexBuffer(const uint32_t* const indices, uint32_t size);
		
		// Inherited via IIndexBuffer
		virtual void Bind() override;
		virtual void Unbind() override;
		inline virtual uint32_t GetSize() const override { return m_size; }
	};
}