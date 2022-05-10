#pragma once

#include "Renderer/VertexArray.h"

namespace kepler {
	// DX11 Vertex Array는 OpenGL의 VAO과 달리 그냥 하나의 VertexBuffer와 IndexBuffer를 함께 관리해 주는 객체로 여길 것.
	class DX11VertexArray : public IVertexArray
	{
	private:
		std::shared_ptr<IVertexBuffer> m_pVertexBuffer;
		std::shared_ptr<IIndexBuffer> m_pIndexBuffer;

	public:
		// Inherited via IVertexArray
		virtual void Bind() override;
		virtual void Unbind() override;

		inline virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& const vertexBuffer) override { m_pVertexBuffer = vertexBuffer; }
		inline virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& const indexBuffer) override { m_pIndexBuffer = indexBuffer; }
		
		inline virtual const std::shared_ptr<IVertexBuffer>& GetVertexBuffer() const override { return m_pVertexBuffer; }
		inline virtual const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override { return m_pIndexBuffer; }
		
		inline virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override 
		{
			return std::vector<std::shared_ptr<IVertexBuffer>>(1, m_pVertexBuffer); 
		}
	};
}