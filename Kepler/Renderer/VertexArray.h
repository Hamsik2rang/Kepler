#pragma once

#include <vector>

#include "Core/Base.h"
#include "Buffer.h"

namespace kepler {

	enum class ePrimitiveType
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

	// Vertex Array Inferface
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> pVertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> pIndexBuffer) = 0;
		virtual void SetInstanceBuffer(const std::shared_ptr<IInstanceBuffer> pInstanceBuffer) = 0;
		virtual void SetPrimitiveType(ePrimitiveType type, uint8_t cpCount = 0) = 0;

		virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IIndexBuffer> GetIndexBuffer() const = 0;
		virtual const ePrimitiveType GetPrimitiveType() const = 0;
		virtual const uint8_t GetControlPatchCount() const = 0;

		static std::shared_ptr<IVertexArray> Create();
	};

}