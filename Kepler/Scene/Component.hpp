#pragma once

#include <initializer_list>
#include <type_traits>

#include "Core/Base.h"
#include "KeplerMath.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "ComponentType.h"


namespace kepler {

	class Entity;

#define COMPONENT_CLASS_TYPE(type) \
	static eComponentType GetStaticType() { return eComponentType::##type; } \
	virtual eComponentType GetType() const override { return GetStaticType(); }

#define COMPONENT_CLASS_INDEX(index) \
	static eComponentIndex GetStaticIndex() { return eComponentIndex::##index; } \
	virtual eComponentIndex GetIndex() const override { return GetStaticIndex(); }

#define COMPONENT_CLASS_FLAG(flag) \
	static eComponentFlag GetStaticFlag() { return eComponentFlag::##flag; } \
	virtual eComponentFlag GetFlag() const override { return GetStaticFlag(); }

	class IComponent
	{
	private:
		Entity* m_pOwner;

	public:
		virtual eComponentType GetType() const = 0;
		virtual eComponentIndex GetIndex() const = 0;
		virtual eComponentFlag GetFlag() const = 0;

		// \brief Component를 지정한 Entity가 소유하도록 합니다.
		// \param pInEntity 이 Component를 소유할 Entity의 주소
		void SetOwner(Entity* pInEntity) { m_pOwner = pInEntity; }

		// \brief 이 Component를 소유한 Entity의 주소를 리턴합니다.
		// \return 소유권을 가진 Entity의 주소
		Entity* GetOwner() { return m_pOwner; }
	};

	class TagComponent : public IComponent
	{
	public:
		std::string name{ "Unknown" };

		TagComponent() = default;
		TagComponent(const TagComponent& _tag) = default;
		TagComponent(const std::string& _name)
			: name{ _name }
		{}

		COMPONENT_CLASS_TYPE(TagComponent)
			COMPONENT_CLASS_INDEX(TagIndex)
			COMPONENT_CLASS_FLAG(TagFlag)
	};

	class TransformComponent : public IComponent
	{
	public:
		Vec3f translation{ 0.0f, 0.0f, 0.0f };
		Vec3f rotation{ 0.0f, 0.0f, 0.0f };
		Vec3f scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& _transform) = default;
		TransformComponent(const Vec3f& _translation, const Vec3f& _rotation, const Vec3f& _scale)
			: translation{ _translation }
			, rotation{ _rotation }
			, scale{ _scale }
		{}

		COMPONENT_CLASS_TYPE(TransformComponent)
			COMPONENT_CLASS_INDEX(TransformIndex)
			COMPONENT_CLASS_FLAG(TransformFlag)
	};

	class MeshRendererComponent : public IComponent
	{
	public:
		std::shared_ptr<IVertexArray> pVertexArray;

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent& _mesh) = default;
		MeshRendererComponent(const std::shared_ptr<IVertexArray>& _pVertexArrray)
			:pVertexArray{ _pVertexArrray }
		{}

		COMPONENT_CLASS_TYPE(MeshRendererComponent)
			COMPONENT_CLASS_INDEX(MeshRendererIndex)
			COMPONENT_CLASS_FLAG(MeshRendererFlag)
	};

	class SpriteRendererComponent : public IComponent
	{
	public:
		std::shared_ptr<ITexture2D> pTexture;
		Vec4f color;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& _sprite) = default;
		SpriteRendererComponent(const std::shared_ptr<ITexture2D>& _pTexture, const Vec4f& _color)
			: pTexture{ _pTexture }
			, color{ _color }
		{}

		COMPONENT_CLASS_TYPE(SpriteRendererComponent)
			COMPONENT_CLASS_INDEX(SpriteRendererIndex)
			COMPONENT_CLASS_FLAG(SpriteRendererFlag)
	};

	template <typename ... TComponent>
	struct TypeToFlag;

	template <>
	struct TypeToFlag<>
	{
		uint64_t  operator()() const
		{
			return 0;
		}
	};

	template <typename TComponent, typename ... TOther>
	struct TypeToFlag<TComponent, TOther...>
	{
		uint64_t operator()() const
		{
			uint64_t bitset = (uint64_t)TComponent::GetStaticFlag() | (uint64_t)TypeToFlag<TOther...>()();
			return bitset;
		}
	};

	template <typename ... TComponent>
	eComponentFlag ConvertTypeToFlag()
	{
		return (eComponentFlag)TypeToFlag<TComponent...>()();
	}
}