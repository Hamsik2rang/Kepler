#include "kepch.h"

#include "DX11VertexArray.h"

namespace kepler {
	
	void DX11VertexArray::Bind()
	{
		m_pVertexBuffer->Bind();
		m_pIndexBuffer->Bind();
	}

	void DX11VertexArray::Unbind()
	{
		m_pVertexBuffer->Unbind();
		m_pIndexBuffer->Unbind();
	}
}