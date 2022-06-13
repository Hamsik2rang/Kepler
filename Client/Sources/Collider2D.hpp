#pragma once

#include "Kepler.h"

enum class eColliderType
{
	None = 0,
	Box,
	Circle,
};

enum class eColliderCategory
{
	Player = 0,
	Enemy,
	Ball,
	Net,
	Ground,
	Wall,
	Sky,
};

class Collider2D
{
protected:
	const eColliderType m_type;
	const eColliderCategory m_category;
	bool m_bIsTrigger;	// 물리 연산을 수행하지 않는 충돌체(트리거)의 경우 true

	GameObject& m_gameObject;

public:
	Collider2D(GameObject& pGameObject, bool bIsTrigger, const eColliderType type, const eColliderCategory category)
		: m_type{ type }
		, m_category{ category }
		, m_bIsTrigger{ bIsTrigger }
		, m_gameObject{ pGameObject }
	{}

	virtual ~Collider2D() = default;

	inline const eColliderType GetType() const { return m_type; }
	inline const eColliderCategory GetCategory() const { return m_category; }
	inline GameObject& GetGameObject() const { return m_gameObject; }
	inline bool IsTrigger() const { return m_bIsTrigger; }	

	virtual void SetPosition(kepler::Vec2f position) = 0;
	virtual void SetSize(kepler::Vec2f size) = 0;

	virtual const kepler::Vec2f GetPosition() const = 0;
	virtual const kepler::Vec2f GetSize() const = 0;
	virtual const kepler::Vec2f GetDirection() const = 0;
	virtual void* GetCollisionInfo() const = 0;
};

class BoxCollider2D : public Collider2D
{
private:
	kepler::Vec2f m_position;
	kepler::Vec2f m_size;

public:
	BoxCollider2D(GameObject& pGameObject, kepler::Vec2f position, kepler::Vec2f size, bool bIsTrigger, const eColliderCategory category)
		: Collider2D(pGameObject, bIsTrigger, eColliderType::Box, category)
		, m_position{ position }
		, m_size{ size }
	{}

	// Inherited via Collier2D
	inline virtual void SetPosition(kepler::Vec2f position) { m_position = position; }
	inline virtual void SetSize(kepler::Vec2f size) { m_size = size; }

	inline virtual const kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual const kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual const kepler::Vec2f GetDirection() const override { return m_gameObject.GetCurrentDirection(); }
	inline virtual void* GetCollisionInfo() const override { return m_gameObject.GetCollisionInfo(); }
};

class CircleCollider2D : public Collider2D
{
private:
	kepler::Vec2f m_position;
	float m_radius;

public:
	CircleCollider2D(GameObject& pGameObject, kepler::Vec2f position, float radius, bool bIsTrigger, const eColliderCategory category)
		: Collider2D(pGameObject, bIsTrigger, eColliderType::Circle, category)
		, m_position{ position }
		, m_radius{ radius }
	{}

	// Inherited via Collier2D
	inline virtual void SetPosition(kepler::Vec2f position) { m_position = position; }
	inline virtual void SetSize(kepler::Vec2f size) { m_radius = size.x; }

	inline virtual const kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual const kepler::Vec2f GetSize() const override { return kepler::Vec2f{ m_radius, m_radius }; }
	inline virtual const kepler::Vec2f GetDirection() const override { return m_gameObject.GetCurrentDirection(); }
	inline virtual void* GetCollisionInfo() const override { return m_gameObject.GetCollisionInfo(); }
};