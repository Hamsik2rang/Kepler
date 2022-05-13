#pragma once

#include "Renderer/VertexArray.h"

namespace kepler {

	class DX11VertexArray : public IVertexArray
	{
	private:
		std::vector<std::shared_ptr<IVertexBuffer>> m_pVertexBuffers;
		std::shared_ptr<IIndexBuffer> m_pIndexBuffer;

	public:
		// Inherited via IVertexArray
		virtual void Bind() override;
		virtual void Unbind() override;

		inline virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& const vertexBuffer) override { m_pVertexBuffers.push_back(vertexBuffer); }
		inline virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& const indexBuffer) override { m_pIndexBuffer = indexBuffer; }
		
		inline virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override { return m_pVertexBuffers; }
		inline virtual const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override { return m_pIndexBuffer; }
	};
}