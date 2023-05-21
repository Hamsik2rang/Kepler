#include "kepch.h"

#include "DX11VertexArray.h"
#include "DX11Context.h"

namespace kepler {
DX11VertexArray::DX11VertexArray()
	: m_pIndexBuffer{ nullptr }
	, m_primitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST }
	, m_cpCount{ 0 }
	, m_primitiveType{ ePrimitiveType::Triangle }
{

}

void DX11VertexArray::Bind()
{
	ID3D11DeviceContext* pContext = static_cast<ID3D11DeviceContext*>(IGraphicsContext::Get()->GetDeviceContext());

	for (uint32_t i = 0; i < m_pVertexBuffers.size(); i++)
	{
		m_pVertexBuffers[i]->Bind();
	}
	m_pIndexBuffer->Bind();
	pContext->IASetPrimitiveTopology(m_primitiveTopology);
}

void DX11VertexArray::Unbind()
{
	for (auto& vb : m_pVertexBuffers)
	{
		vb->Unbind();
	}
	m_pIndexBuffer->Unbind();
}

void DX11VertexArray::SetPrimitiveType(ePrimitiveType type, uint8_t cpCount)
{
	m_primitiveType = type;
	m_cpCount = cpCount;
	switch (m_primitiveType)
	{
	case ePrimitiveType::Point:				m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case ePrimitiveType::Line:				m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case ePrimitiveType::LineAdj:			m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; break;
	case ePrimitiveType::LineStrip:			m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
	case ePrimitiveType::LineStripAdj:		m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ; break;
	case ePrimitiveType::Triangle:			m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case ePrimitiveType::TriangleAdj:		m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ; break;
	case ePrimitiveType::TriangleStrip:		m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	case ePrimitiveType::TriangleStripAdj:	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ; break;
	case ePrimitiveType::CPPatch:
		m_primitiveTopology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + m_cpCount); break;
	default:
	{
		KEPLER_CORE_ASSERT(false, "None Primitive type is not supported");
	}
	}
}
}