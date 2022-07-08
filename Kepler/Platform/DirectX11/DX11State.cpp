#include "kepch.h"

#include "DX11State.h"
#include "Renderer/GraphicsContext.h"

namespace kepler {

	DX11State::DX11State()
		: m_desc{}
		, m_bIsChanged{ true }
	{
		m_desc.bCullBackFace = true;
		m_desc.bIsFrontClockwise = true;
		m_desc.bDepthTest = true;
		m_desc.bDepthWrite = false;
		m_desc.bWireFrame = false;
		m_desc.lineWidth = 1.5f;
	}

	void DX11State::SetRasterizerState()
	{
		D3D11_RASTERIZER_DESC desc{};
		if (m_desc.bCullBackFace) desc.CullMode = D3D11_CULL_BACK;
		else desc.CullMode = D3D11_CULL_FRONT;

		if (m_desc.bIsFrontClockwise) desc.FrontCounterClockwise = false;
		else desc.FrontCounterClockwise = true;

		if (m_desc.bWireFrame) desc.FillMode = D3D11_FILL_WIREFRAME;
		else desc.FillMode = D3D11_FILL_SOLID;


	}

	void DX11State::SetBlendState()
	{

	}

	void DX11State::SetDepthStencilState()
	{

	}

	void DX11State::Bind()
	{
		if (m_bIsChanged)
		{
			//...


			m_bIsChanged = false;
		}
	}

}