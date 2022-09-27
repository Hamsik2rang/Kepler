#pragma once

#include <string>
#include <initializer_list>
#include "Core/Base.h"

namespace kepler {

	// �߻�ȭ�� ���̴� ������ Ÿ��
	enum class EShaderDataType
	{
		None = 0,
		Bool,

		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,

		Float33,
		Float44,

		Float1 = Float,
		Int1 = Int,
		Matrix = Float44
	};
	// Buffer Usage
	enum class EBufferUsage
	{
		Default = 0,
		Static,
		Dynamic,
		Immutable,
		Staging,
	};

	// �߻�ȭ�� ���̴� ������ Ÿ�Կ� ���� Ÿ�� ũ�⸦ �����ϴ� �Լ�
	static uint32_t ShaderDataTypeSize(EShaderDataType type)
	{
		switch (type)
		{
		case EShaderDataType::Bool:		return 1;
		case EShaderDataType::Float:	return 4;
		case EShaderDataType::Float2:	return 4 * 2;
		case EShaderDataType::Float3:	return 4 * 3;
		case EShaderDataType::Float4:	return 4 * 4;
		case EShaderDataType::Int:		return 4;
		case EShaderDataType::Int2:		return 4 * 2;
		case EShaderDataType::Int3:		return 4 * 3;
		case EShaderDataType::Int4:		return 4 * 4;
		case EShaderDataType::Float33:	return 4 * 3 * 3;
		case EShaderDataType::Float44:	return 4 * 4 * 4;
		}

		KEPLER_ASSERT(false, "Invalid Shader Datatype.");
		return 0;
	}

	// BufferLayout�� ���� ����
	struct BufferElement
	{
		std::string name;
		uint32_t index;
		uint32_t size;
		uint32_t offset;
		EShaderDataType type;

		BufferElement() = default;
		BufferElement(const std::string& _name, const uint32_t _index, EShaderDataType _type, uint32_t offset, uint32_t stride)
			: name{ _name }
			, index{ _index }
			, type{ _type }
			, size{ ShaderDataTypeSize(_type) }
			, offset{ 0 }
		{}

		inline uint32_t GetComponentCount() const
		{
			switch (type)
			{
			case EShaderDataType::Bool:
			case EShaderDataType::Int:
			case EShaderDataType::Float:	return 1;
			case EShaderDataType::Int2:
			case EShaderDataType::Float2:	return 2;
			case EShaderDataType::Int3:
			case EShaderDataType::Float3:
			case EShaderDataType::Float33:	return 3; // FLOAT3 * 3
			case EShaderDataType::Int4:
			case EShaderDataType::Float4:
			case EShaderDataType::Float44:	return 4; // FLOAT4(or VECTOR4) * 4
			}

			KEPLER_ASSERT(false, "Invalid Shader Datatype.");
			return 0;
		}
	};

	// Vertex Buffer�� ���� Buffer Layout(Vertex Shader�� Input���� �����)
	// Vertex Buffer�� �����Ͱ� � Element�� �����Ǿ� �ִ���
	// Ȥ�� Element���� ��� Chunking�Ǿ� �ִ��� �����ϴ� ��ü
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;

		inline void CalcOffsetAndStride()
		{
			uint32_t offset = 0u;
			m_stride = 0;
			for (auto& e : m_elements)
			{
				e.offset = offset;
				offset += e.size;
				m_stride += e.size;
			}
		}
	public:
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements{ elements }
		{
			CalcOffsetAndStride();
		}

		inline size_t GetElementCount() const { return m_elements.size(); }
		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		[[nodiscard]] std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	};

	// Vertex Buffer Interface
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& bufferLayout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static std::shared_ptr<IVertexBuffer> Create(const uint32_t size, EBufferUsage usage);
		static std::shared_ptr<IVertexBuffer> Create(const void* const vertices, const uint32_t size, EBufferUsage usage);
	};

	// Index Buffer Interface
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetCount() const = 0;

		static std::shared_ptr<IIndexBuffer> Create(const uint32_t* const indices, const uint32_t count, EBufferUsage usage);
	};

	// Instance Buffer Interface
	class IInstanceBuffer
	{
	public:
		virtual ~IInstanceBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(const void* data, uint32_t size, uint32_t count) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual uint32_t GetCount() const = 0;
		
		//! ���� Graphics API�� �´� Instance Buffer�� �����մϴ�.
		//! \param size : �ν��Ͻ� �������� ��ü ũ��.
		//! \param count : �ν��Ͻ� ��ü ��.
		//! \param usage : Buffer usage(default/dynamic/static/staging).
		static std::shared_ptr<IInstanceBuffer> Create(const uint32_t size, const uint32_t count, EBufferUsage usage);

		//! ���� Graphics API�� �´� Instance Buffer�� �����մϴ�.
		//! \param data : �ν��Ͻ� ������
		//! \param size : �ν��Ͻ� �������� ��ü ũ��.
		//! \param count : �ν��Ͻ� ��ü ��.
		//! \param usage : Buffer usage(default/dynamic/static/staging).
		static std::shared_ptr<IInstanceBuffer> Create(const void* const data, const uint32_t size, const uint32_t count, EBufferUsage usage);
	};
}