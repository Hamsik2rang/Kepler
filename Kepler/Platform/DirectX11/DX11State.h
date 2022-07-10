#pragma once

#include "Renderer/RenderState.h"

namespace kepler {

	class DX11State : public IRenderState
	{
	private:
		ID3D11RasterizerState* m_pRasterizerState;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11BlendState* m_pBlendState;

		bool m_bHasChanged;

		void SetRasterizerState();
		void SetDepthStencilState();
		void SetBlendState();

	public:
		DX11State();

		virtual ~DX11State();

		virtual void SetShaderState(ShaderStateDescription& desc);
		virtual void SetShaderState(const eShaderType& type, const std::shared_ptr<IShader>& pShader);
		virtual void SetRasterizerState(RasterizerStateDescription& desc);
		virtual void SetDepthState(DepthStateDescription& desc);
		virtual void SetStencilState(StencilStateDescription& desc);
		virtual void SetBlendState(BlendStateDescription& desc);

		// Inherited via IRenderState
		virtual void Bind() override;
	};
}