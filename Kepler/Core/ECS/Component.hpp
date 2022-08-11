#pragma once

#include "Core/Base.h"
#include "KeplerMath.h"
#include "ComponentType.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

namespace kepler {
	
	class Entity;
	
	namespace component {



		struct IComponent
		{
			
		};

		struct Tag : public IComponent
		{
			std::string tag;

			Tag() = default;
			Tag(const Tag&) = default;
			Tag(std::string& _tag)
				: tag{ _tag }
			{}

		};

		struct Transform : public IComponent
		{
			Vec3f translation{0.0f, 0.0f, 0.0f};
			Vec3f rotation{0.0f, 0.0f, 0.0f};
			Vec3f scale{1.0f, 1.0f, 1.0f};

			Transform() = default;
			Transform(const Transform&) = default;
			Transform(const Vec3f& _translation)
				: translation{ _translation }
			{}
		};

		struct MeshRenderer : public IComponent
		{
			std::shared_ptr<IVertexArray> pVertexArray;

			MeshRenderer() = default;
			MeshRenderer(const MeshRenderer&) = default;
			MeshRenderer(const std::shared_ptr<IVertexArray>& _pVA)
				: pVertexArray{ _pVA }
			{}
		};

		struct SpriteRenderer : public IComponent
		{
			std::shared_ptr<ITexture2D> pTexture;
		};

		struct Camera : public IComponent
		{
			bool bIsMainCamera;
			bool bIsAspectFixed;
		
			Camera() = default;
			Camera(const Camera&) = default;
		};
	}
}