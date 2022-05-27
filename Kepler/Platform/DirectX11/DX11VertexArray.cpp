#include "kepch.h"

#include "DX11VertexArray.h"

namespace kepler {
	
	void DX11VertexArray::Bind()
	{
		for (uint32_t i = 0; i < m_pVertexBuffers.size(); i++)
		{
			m_pVertexBuffers[i]->Bind();
		}
		m_pIndexBuffer->Bind();
	}

	void DX11VertexArray::Unbind()
	{
		for (auto& vb : m_pVertexBuffers)
		{
			vb->Unbind();
		}
		m_pIndexBuffer->Unbind();
	}

	void DX11VertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer> vertexBuffer)
	{ 
		m_pVertexBuffers.push_back(vertexBuffer); 
	}

	void DX11VertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer> indexBuffer)
	{ 
		m_pIndexBuffer = indexBuffer; 
	}
}