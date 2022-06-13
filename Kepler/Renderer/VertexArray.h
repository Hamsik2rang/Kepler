#pragma once

#include <vector>

#include "Core/Base.h"
#include "Buffer.h"

namespace kepler {

	// Vertex Array Inferface
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IIndexBuffer> GetIndexBuffer() const = 0;

		static std::shared_ptr<IVertexArray> Create();
	};

}