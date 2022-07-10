#pragma once

#include "Renderer/RenderState.h"

namespace kepler {

	class DX11State : public IRenderState
	{
	private:
		ID3D11RasterizerState* m_pRasterizerState;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11BlendState* m_pBlendState;

		bool m_bShaderStateChaged;
		bool m_bRasterizerStateChanged;
		bool m_bDepthStateChanged;
		bool m_bStencilStateChaged;
		bool m_bBlendStateChaged;

		void CreateRasterizerState();
		void CreateDepthStencilState();
		void CreateBlendState();
	public:
		DX11State();

		virtual ~DX11State();

		virtual void SetShaderState(const eShaderType& type, const std::shared_ptr<IShader>& pShader);
		inline virtual void SetShaderState(ShaderStateDescription& desc) { s_shaderDesc = desc; }
		inline virtual void SetRasterizerState(RasterizerStateDescription& desc) { s_rasterizerDesc = desc; }
		inline virtual void SetDepthState(DepthStateDescription& desc) { s_depthDesc = desc; }
		inline virtual void SetStencilState(StencilStateDescription& desc) { s_stencilDesc = desc; }
		inline virtual void SetBlendState(BlendStateDescription& desc) { s_blendDesc = desc; }

		// Inherited via IRenderState
		virtual void Bind() override;
	};
}