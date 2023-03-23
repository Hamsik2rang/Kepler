#pragma once

#include <initializer_list>
#include <type_traits>

#include "Core/Base.h"
#include "KeplerMath.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Texture.h"
#include "ComponentType.h"


namespace kepler {

class Entity;

class IComponent
{
private:
	Entity* m_pOwner;

public:
	virtual EComponentType GetType() const = 0;
	virtual ComponentIndex GetIndex() const = 0;
	virtual ComponentFlag GetFlag() const = 0;

	IComponent(Entity* pEntity) : m_pOwner{ pEntity } {}
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

	TagComponent(Entity* pOwner)
		: IComponent(pOwner)
	{}
	TagComponent(Entity* pOwner, const TagComponent& _tag)
		: name{ _tag.name }
		, IComponent(pOwner)
	{}
	TagComponent(Entity* pOwner, const std::string& _name)
		: name{ _name }
		, IComponent(pOwner)
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

	TransformComponent(Entity* pOwner)
		: IComponent(pOwner)
	{}
	TransformComponent(Entity* pOwner, const TransformComponent& _transform)
		: translation{ _transform.translation }
		, rotation{ _transform.rotation }
		, scale{ _transform.scale }
		, IComponent(pOwner)
	{}
	TransformComponent(Entity* pOwner, const Vec3f& _translation, const Vec3f& _rotation, const Vec3f& _scale)
		: translation{ _translation }
		, rotation{ _rotation }
		, scale{ _scale }
		, IComponent(pOwner)
	{}

	COMPONENT_CLASS_TYPE(TransformComponent)
		COMPONENT_CLASS_INDEX(TransformIndex)
		COMPONENT_CLASS_FLAG(TransformFlag)
};

class MeshRendererComponent : public IComponent
{
public:
	std::shared_ptr<IVertexArray> pVertexArray;

	MeshRendererComponent(Entity* pOwner)
		: IComponent(pOwner)
	{}
	MeshRendererComponent(Entity* pOwner, const MeshRendererComponent& _mesh)
		: pVertexArray{ _mesh.pVertexArray }
		, IComponent(pOwner)
	{}
	MeshRendererComponent(Entity* pOwner, const std::shared_ptr<IVertexArray>& _pVertexArrray)
		: pVertexArray{ _pVertexArrray }
		, IComponent(pOwner)
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

	SpriteRendererComponent(Entity* pOwner)
		: IComponent(pOwner)
	{}
	SpriteRendererComponent(Entity* pOwner, const SpriteRendererComponent& _sprite)
		: pTexture{ _sprite.pTexture }
		, color{ _sprite.color }
		, IComponent(pOwner)
	{}
	SpriteRendererComponent(Entity* pOwner, const std::shared_ptr<ITexture2D>& _pTexture, const Vec4f& _color)
		: pTexture{ _pTexture }
		, color{ _color }
		, IComponent(pOwner)
	{}

	COMPONENT_CLASS_TYPE(SpriteRendererComponent)
		COMPONENT_CLASS_INDEX(SpriteRendererIndex)
		COMPONENT_CLASS_FLAG(SpriteRendererFlag)
};

class ScriptableEntity;

class NativeScriptableComponent : public IComponent
{
	ScriptableEntity* pInstance;

	std::function<ScriptableEntity* ()> Instantiate;
	std::function<void()> Destroy;

	template <typename T>
	void Bind()
	{
		Instantiate = []()->ScriptableEntity* { return new T; };
		Destroy = []()->void { delete dynamic_cast<T*>(pInstance); pInstance = nullptr; };
	}
};
}