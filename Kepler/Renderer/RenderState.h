#pragma once

#include <memory>

#include "Core/Base.h"
#include "Shader.h"

namespace kepler {

	enum class eDepthComparer
	{
		None = 0,
		Never,
		Less,
		LessOrEqual,
		Equal,
		NotEqual,
		Greater,
		GreaterOrEqual,
		Always,
	};

	//----- Shader State------------------
	struct ShaderStateDescription
	{
		std::shared_ptr<IShader> pVertexShader;
		std::shared_ptr<IShader> pHullShader;
		std::shared_ptr<IShader> pDomainShader;
		std::shared_ptr<IShader> pGeometryShader;
		std::shared_ptr<IShader> pPixelShader;
	};
	//------------------------------------

	//----- Rasterizer State--------------
	struct RasterizerStateDescription
	{
		bool bCullBackFace;
		bool bIsFrontClockwise;
		bool bWireFrame;
	};
	//------------------------------------

	//----- Z-Buffer State----------------
	struct DepthStateDescription
	{
		eDepthComparer depthComparer;
		bool bDepthTest;
		bool bDepthWrite;
		uint8_t depthReadMask;
		uint8_t depthWriteMask;
	};
	//------------------------------------

	//----- Stencil Buffer State----------
	struct StencilStateDescription
	{
		bool bStencilTest;
		bool bStencilWrite;
		uint8_t stencilReadMask;
		uint8_t stencilWriteMask;
	};
	//------------------------------------

	//----- Blend State-------------------
	struct BlendStateDescription
	{
		bool bBlendEnable;
	};
	//------------------------------------

	class IRenderState
	{
	private:
		static std::shared_ptr<IRenderState> s_pInstance;
	
	protected:
		static ShaderStateDescription s_shaderDesc;
		static RasterizerStateDescription s_rasterizerDesc;
		static DepthStateDescription s_depthDesc;
		static StencilStateDescription s_stencilDesc;
		static BlendStateDescription s_blendDesc;

	public:
		virtual ~IRenderState() = default;

		virtual const ShaderStateDescription& GetShaderState() const { return s_shaderDesc; }
		virtual const RasterizerStateDescription& GetRasterizer() const { return s_rasterizerDesc; }
		virtual const DepthStateDescription& GetDepthState() const { return s_depthDesc; }
		virtual const StencilStateDescription& GetStencilState() const { return s_stencilDesc; }
		virtual const BlendStateDescription& GetBlendState() const { return s_blendDesc; }
		
		virtual void SetShaderState(ShaderStateDescription& desc) = 0;
		virtual void SetShaderState(const eShaderType& type, const std::shared_ptr<IShader>& pShader) = 0;
		virtual void SetRasterizerState(RasterizerStateDescription& desc) = 0;
		virtual void SetDepthState(DepthStateDescription& desc) = 0;
		virtual void SetStencilState(StencilStateDescription& desc) = 0;
		virtual void SetBlendState(BlendStateDescription& desc) = 0;
		
		virtual void Bind() = 0;

		static std::shared_ptr<IRenderState> Create();
		inline static std::shared_ptr<IRenderState> Get() { return s_pInstance; }
	};
}