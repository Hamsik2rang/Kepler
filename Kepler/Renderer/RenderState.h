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
		Triangle,
		LineStrip,
		TriangleStrip,
	};

	enum class eDepthComparer
	{
		None = 0,
		Never,
		NotEqual,
		Less,
		LessOrEqual,
		Equal,
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
		//TODO: add additional states
		
		bool bCullBackFace;
		bool bDepthTest;
		bool bDepthWrite;
		bool bWireFrame;
		float lineWidth;
	};

	class IRenderState
	{
	private:
		static std::shared_ptr<IRenderState> s_pInstance;

	public:
		virtual ~IRenderState() = 0;

		virtual void Bind() = 0;
		virtual void GetStateDescription() = 0;
		
		static std::shared_ptr<IRenderState> Create();
	};
}