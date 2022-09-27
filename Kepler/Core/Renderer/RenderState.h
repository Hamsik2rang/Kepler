#pragma once

#include <memory>

#include "Core/Base.h"
#include "Shader.h"
#include "FrameBuffer.h"

namespace kepler {

	//----- Enum Types--------------------
	// TODO: ���� Stencil Comparer�� Depth Comparer�� �ٸ� API�� ������ ������ ��� ������ �и��ؾ� ��
	enum class ECullMode
	{
		None = 0,
		Off,
		Front,
		Back
	};

	enum class EFrameBufferComparer
	{
		None = 0,
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always,
	};
	using EDepthComparer = EFrameBufferComparer;
	using EStencilComparer = EFrameBufferComparer;

	enum class EStencilOperator
	{
		None = 0,
		Keep,
		Zero,
		Replace,
		IncreaseAndClamp,
		DecreaseAndClamp,
		Invert,
		Increase,
		Decrease,
	};

	enum class EBlendFactor
	{
		None = 0,
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha,
		DestAlpha,
		InvDestAlpha,
		DestColor,
		InvDestColor,
		SrcAlphaClamp,
		CustomFactor,
		InvCustomFactor,
		DualSrcColor,
		DualInvSrcColor,
		DualSrcAlpha,
		DualInvSrcAlpha
	};

	enum class EBlendOperator
	{
		None = 0,
		Add,
		Sub,
		RevSub,
		Min,
		Max
	};
	//------------------------------------

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
		ECullMode cullMode;
		bool bIsFrontClockwise;
		bool bWireFrame;
		bool bScissssor;
		bool bMultiSampling;
		bool bAntiAliasedLine;
		bool bDepthClip;
		int depthBias;
		float depthSlopedBias;
		float depthClampBias;
	};
	//------------------------------------

	//----- Z-Buffer State----------------
	struct DepthStateDescription
	{
		bool bDepthTest;
		bool bDepthWrite;
		uint8_t readMask;
		uint8_t writeMask;
		EDepthComparer comparer;
	};
	//------------------------------------

	//----- Stencil Buffer State----------
	struct StencilStateDescription
	{
		bool	bStencilTest;
		uint8_t	readMask;
		uint8_t	writeMask;
		uint32_t stencilRef;

		EStencilOperator frontFailOperator;
		EStencilOperator frontFailDepthOperator;
		EStencilOperator frontPassOperator;

		EStencilOperator backFailOperation;
		EStencilOperator backFailDepthOperation;
		EStencilOperator backPassOperation;

		EStencilComparer comparer;
	};
	//------------------------------------

	//----- Blend State-------------------
	struct BlendStateDescription
	{
		bool bUseAlphaToCoverage;
		bool bIndependentBlend;
		// TODO: ���� FrameBuffer ��ü�� ����� �Ǹ� ���� ���踦 �ٽ� ������ ����(�� FrameBuffer�� �� ��ũ������ ������ �ϰ� BlendStateDesc�� FrameBuffer �����͸� ������ �� �� ����)
		struct RenderTargetDescription
		{
			bool bBlendEnable;
			uint8_t writeMask;

			EBlendFactor srcColorFactor;
			EBlendFactor destColorFactor;
			EBlendFactor srcAlphaFactor;
			EBlendFactor destAlphaFactor;

			EBlendOperator colorBlendOperator;
			EBlendOperator alphaBlendOperator;
		} renderTarget[8];
		float customFactor[4];
		uint32_t sampleMask;
	};
	//------------------------------------

	class IRenderState
	{
	private:
		static std::shared_ptr<IRenderState> s_pInstance;

	protected:
		static ShaderStateDescription		s_shaderDesc;
		static RasterizerStateDescription	s_rasterizerDesc;
		static DepthStateDescription		s_depthDesc;
		static StencilStateDescription		s_stencilDesc;
		static BlendStateDescription		s_blendDesc;

	public:
		virtual ~IRenderState() = default;

		virtual const ShaderStateDescription& GetShaderState() const { return s_shaderDesc; }
		virtual const RasterizerStateDescription& GetRasterizerState() const { return s_rasterizerDesc; }
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