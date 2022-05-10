#pragma once

#include <string>
#include <initializer_list>
#include "Core/Base.h"

namespace kepler {

	// 추상화된 쉐이더 데이터 타입
	enum class eShaderDataType
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

	// 추상화된 쉐이더 데이터 타입에 대한 타입 크기를 리턴하는 함수
	static uint32_t ShaderDataTypeSize(eShaderDataType type)
	{
		switch (type)
		{
		case eShaderDataType::Bool:		return 1;
		case eShaderDataType::Float:	return 4;
		case eShaderDataType::Float2:	return 4 * 2;
		case eShaderDataType::Float3:	return 4 * 3;
		case eShaderDataType::Float4:	return 4 * 4;
		case eShaderDataType::Int:		return 4;
		case eShaderDataType::Int2:		return 4 * 2;
		case eShaderDataType::Int3:		return 4 * 3;
		case eShaderDataType::Int4:		return 4 * 4;
		case eShaderDataType::Float33:	return 4 * 3 * 3;
		case eShaderDataType::Float44:	return 4 * 4 * 4;
		}

		KEPLER_ASSERT(false, "Invalid Shader Datatype.");
		return 0;
	}

	// InputLayout의 구성 원소
	struct InputElement
	{
		std::string name;
		uint32_t index;
		uint32_t size;
		uint32_t stride;
		size_t offset;
		eShaderDataType type;

		InputElement() = default;
		InputElement(const std::string& _name, const uint32_t _index, eShaderDataType _type)
			: name{ _name }
			, index{_index}
			, type{ _type }
			, size{ ShaderDataTypeSize(_type)}
			, stride{ 0 }
			, offset{ 0 }
		{}

		inline uint32_t GetComponentCount() const
		{
			switch (type)
			{
			case eShaderDataType::Bool:
			case eShaderDataType::Int:
			case eShaderDataType::Float:	return 1;
			case eShaderDataType::Int2:
			case eShaderDataType::Float2:	return 2;
			case eShaderDataType::Int3:
			case eShaderDataType::Float3:
			case eShaderDataType::Float33:	return 3; // FLOAT3 * 3
			case eShaderDataType::Int4:
			case eShaderDataType::Float4:
			case eShaderDataType::Float44:	return 4; // FLOAT4(or VECTOR4) * 4
			}

			KEPLER_ASSERT(false, "Invalid Shader Datatype.");
			return 0;
		}
	};

	// Vertex Buffer를 위한 Input Layout(Vertex Shader의 Input으로 연결됨)
	class InputLayout
	{
	private:
		std::vector<InputElement> m_elements;
		uint32_t m_stride;

		inline void CalcOffsetAndStride()
		{
			size_t offset = 0u;
			m_stride = 0;
			for (auto& e : m_elements)
			{
				e.offset = offset;
				offset += e.size;
				m_stride += e.size;
			}
		}
	public:
		InputLayout(std::initializer_list<InputElement> elements)
			: m_elements{elements}
		{
			CalcOffsetAndStride();
		}

		inline size_t GetElementCount() const { return m_elements.size(); }
		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<InputElement>& GetElements() const { return m_elements; }

		[[nodiscard]] std::vector<InputElement>::iterator begin() { return m_elements.begin(); }
		[[nodiscard]] std::vector<InputElement>::iterator end() { return m_elements.end(); }
		[[nodiscard]] std::vector<InputElement>::const_iterator begin() const { return m_elements.begin(); }
		[[nodiscard]] std::vector<InputElement>::const_iterator end() const { return m_elements.end(); }
	};

	// Vertex Buffer Interface
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;
	
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(const InputLayout& layout) = 0;
		virtual const InputLayout& GetLayout() const = 0;
	
		static std::shared_ptr<IVertexBuffer> Create(const uint32_t size);
		static std::shared_ptr<IVertexBuffer> Create(const float* const vertices, const uint32_t size);
	};

	// Index Buffer Interface
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetSize() const = 0;

		static std::shared_ptr<IIndexBuffer> Create(const uint32_t* const indices, const uint32_t size);
	};
}