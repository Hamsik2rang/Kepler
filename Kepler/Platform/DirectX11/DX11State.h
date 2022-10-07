#pragma once

#include "Core/Renderer/RenderState.h"
#include "DX11FrameBuffer.h"

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

		virtual void SetShaderState(const EShaderType& type, const std::shared_ptr<IShader>& pShader);
		inline virtual void SetShaderState(ShaderStateDescription& desc) { s_shaderDesc = desc; m_bShaderStateChaged = true; }
		inline virtual void SetRasterizerState(RasterizerStateDescription& desc) { s_rasterizerDesc = desc; m_bRasterizerStateChanged = true; }
		inline virtual void SetDepthState(DepthStateDescription& desc) { s_depthDesc = desc; m_bDepthStateChanged = true; }
		inline virtual void SetStencilState(StencilStateDescription& desc) { s_stencilDesc = desc; m_bStencilStateChaged = true; }
		inline virtual void SetBlendState(BlendStateDescription& desc) { s_blendDesc = desc; m_bBlendStateChaged = true; }

		// Inherited via IRenderState
		virtual void Bind() override;
	};
}