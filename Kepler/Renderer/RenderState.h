#pragma once

#include <memory>

#include "Core/Base.h"
#include "Shader.h"

namespace kepler {

	enum class ePrimitiveTopology
	{
		None = 0,
		Point,
		Line,
		LineAdj,
		LineStrip,
		LineStripAdj,
		Triangle,
		TriangleAdj,
		TriangleStrip,
		TriangleStripAdj,
		CPPatch,
	};

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

	struct RenderStateDescription
	{
		std::shared_ptr<IShader> pVertexShader;
		std::shared_ptr<IShader> pHullShader;
		std::shared_ptr<IShader> pDomainShader;
		std::shared_ptr<IShader> pGeometryShader;
		std::shared_ptr<IShader> pPixelShader;

		ePrimitiveTopology primitiveTopology;
		eDepthComparer depthComparer;
		uint32_t cpCount;

		//TODO: add additional states
		bool bCullBackFace;
		bool bIsFrontClockwise;
		bool bWireFrame;
		bool bDepthTest;
		bool bDepthWrite;
		bool bBlendEnable;
	};

	class IRenderState
	{
	private:
		static std::shared_ptr<IRenderState> s_pInstance;

	public:
		virtual ~IRenderState() = default;

		virtual RenderStateDescription GetDescription() = 0;
		virtual void SetDescription(RenderStateDescription& desc) = 0;
		virtual void Bind() = 0;

		static std::shared_ptr<IRenderState> Create();
		inline static std::shared_ptr<IRenderState> Get() { return s_pInstance; }
	};
}