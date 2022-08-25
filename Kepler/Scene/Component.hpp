#pragma once

#include "Core/Base.h"
#include "KeplerMath.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "ComponentType.h"

namespace kepler {
	namespace component {

#define COMPONENT_CLASS_TYPE(type) \
	static eComponentType GetStaticType() { return eComponentType::##type; } \
	static eComponentIndex GetStaticIndex() { return eComponentIndex::##type; } \
	virtual eComponentType GetType() const override { return GetStaticType(); }

		struct Base
		{
			virtual eComponentType GetType() const = 0;
		};

		struct Tag : public Base
		{
			std::string name{ "Unknown" };

			Tag() = default;
			Tag(const Tag& _tag) = default;
			Tag(const std::string& _name)
				: name{ _name }
			{}

			COMPONENT_CLASS_TYPE(Tag)
		};

		struct Transform : public Base
		{
			Vec3f translation{ 0.0f, 0.0f, 0.0f };
			Vec3f rotation{ 0.0f, 0.0f, 0.0f };
			Vec3f scale{ 1.0f, 1.0f, 1.0f };

			Transform() = default;
			Transform(const Transform& _transform) = default;
			Transform(const Vec3f& _translation, const Vec3f& _rotation, const Vec3f& _scale)
				: translation{ _translation }
				, rotation{ _rotation }
				, scale{ _scale }
			{}

			COMPONENT_CLASS_TYPE(Transform)
		};

		struct MeshRenderer : public Base
		{
			std::shared_ptr<IVertexArray> pVertexArray;

			MeshRenderer() = default;
			MeshRenderer(const MeshRenderer& _mesh) = default;
			MeshRenderer(const std::shared_ptr<IVertexArray>& _pVertexArrray)
				:pVertexArray{ _pVertexArrray }
			{}

			COMPONENT_CLASS_TYPE(MeshRenderer)
		};

		struct SpriteRenderer : public Base
		{
			std::shared_ptr<ITexture2D> pTexture;
			Vec4f color;

			SpriteRenderer() = default;
			SpriteRenderer(const SpriteRenderer& _sprite) = default;
			SpriteRenderer(const std::shared_ptr<ITexture2D>& _pTexture, const Vec4f& _color)
				: pTexture{ _pTexture }
				, color{ _color }
			{}

			COMPONENT_CLASS_TYPE(SpriteRenderer)
		};
	}
}