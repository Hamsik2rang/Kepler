#pragma once

#include "Core/Base.h"

#include "Renderer/VertexArray.h"

namespace kepler {

	class DX11VertexArray : public IVertexArray
	{
	private:
		std::vector<std::shared_ptr<IVertexBuffer>> m_pVertexBuffers;
		std::shared_ptr<IIndexBuffer> m_pIndexBuffer;
		std::shared_ptr<IInstanceBuffer> m_pInstanceBuffer;

		ePrimitiveType m_primitiveType;
		uint8_t m_cpCount;
		D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;

	public:
		DX11VertexArray();

		// Inherited via IVertexArray
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetPrimitiveType(ePrimitiveType type, uint8_t cpCount) override;
		inline virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> pVertexBuffer) override { m_pVertexBuffers.push_back(pVertexBuffer); }
		inline virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> pIndexBuffer) override { m_pIndexBuffer = pIndexBuffer; }
		inline virtual void SetInstanceBuffer(const std::shared_ptr<IInstanceBuffer> pInstanceBuffer) override { m_pInstanceBuffer = pInstanceBuffer; }

		inline virtual const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override { return m_pVertexBuffers; }
		inline virtual const std::shared_ptr<IIndexBuffer> GetIndexBuffer() const override { return m_pIndexBuffer; }
		inline virtual const ePrimitiveType GetPrimitiveType() const override { return m_primitiveType; }
		inline virtual const uint8_t GetControlPatchCount() const override { return m_cpCount; }
	};
}