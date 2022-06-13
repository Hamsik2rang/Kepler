#pragma once

#include <deque>

#include "Kepler.h"
#include "GameObject.h"
#include "CollisionDetector.h"

class Ball : public GameObject
{
private:
	std::deque<std::pair<kepler::Vec2f, float>> m_transforms;
	kepler::Vec2f m_size;
	kepler::Vec2f m_curDirection;
	kepler::Vec2f m_lastDirection;

	std::shared_ptr<kepler::ITexture2D> m_pBallSprite;
	std::shared_ptr<kepler::ITexture2D> m_pImpactSprite;
	CircleCollider2D* m_pCollider;

#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

	bool m_bIsAccelarated;
	bool m_bIsGrounded;
public:
	Ball(kepler::Vec2f position, float radius, eColliderType type = eColliderType::Circle, eColliderCategory category = eColliderCategory::Ball);
	~Ball();
	void Init();

	void OnEvent(kepler::Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();

	virtual void OnCollision(CollisionData& data);

	// Inherited via GameObject
	inline virtual kepler::Vec2f GetPosition() const override { return m_transforms.front().first; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return m_curDirection; }
	inline virtual kepler::Vec2f GetLastDirection() const override { return m_lastDirection; }
	inline virtual void* GetCollisionInfo() const override { return (void*)&m_bIsAccelarated; }
};