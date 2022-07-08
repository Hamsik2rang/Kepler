#pragma once

#include "Renderer/RenderState.h"

namespace kepler {

	class DX11State : public IRenderState
	{
	private:
		RenderStateDescription		m_desc;
		ID3D11RasterizerState*		m_pRasterizerState;
		ID3D11DepthStencilState*	m_pDepthStencilState;
		ID3D11BlendState*			m_pBlendState;
	
		bool m_bIsChanged;

		void SetRasterizerState();
		void SetDepthStencilState();
		void SetBlendState();

	public:
		DX11State();

		virtual ~DX11State() = default;

		// Inherited via IRenderState
		virtual void Bind() override;
		inline virtual RenderStateDescription GetDescription() override { return m_desc; }
		inline virtual void SetDescription(RenderStateDescription& desc) override { m_desc = desc; m_bIsChanged = true; }
	};
}