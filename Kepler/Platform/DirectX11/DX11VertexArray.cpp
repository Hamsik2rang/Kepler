#include "kepch.h"

#include "DX11VertexArray.h"

namespace kepler {
	
	void DX11VertexArray::Bind()
	{
		for (auto& vb : m_pVertexBuffers)
		{
			vb->Bind();
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
}