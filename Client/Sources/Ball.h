#pragma once

#include <deque>

#include "Kepler.h"
#include "GameObject.h"
#include "CollisionDetector.h"

class Ball : public GameObject
{
private:
	std::deque<std::pair<kepler::Vec2f, float>> m_transforms;	// 스파이크 처리 시 공의 궤적으로 잔상을 표현해야 하므로 이전 프레임의 위상을 deque로 저장함.
	kepler::Vec2f m_size;
	kepler::Vec2f m_curDirection;
	kepler::Vec2f m_lastDirection;
	float m_deltaTime;

	std::shared_ptr<kepler::ITexture2D> m_pBallSprite;		// 공 스프라이트
	std::shared_ptr<kepler::ITexture2D> m_pImpactSprite;	// 땅에 닿았을 때의 이펙트 스프라이트
	CircleCollider2D* m_pCollider;

#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

	bool m_bIsAccelarated;	// 스파이크 공격이 진행중인 경우 true
	bool m_bIsGrounded;		// 땅에 닿은 경우 true
public:
	Ball(kepler::Vec2f position, float radius, eColliderType type = eColliderType::Circle, eColliderCategory category = eColliderCategory::Ball);
	~Ball();
	void InitSprite();

	void OnEvent(kepler::Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();

	void Respawn(bool bSpawnAbovePlayer);
	inline bool IsGrounded() const { return m_bIsGrounded; }

	// Inherited via GameObject
	virtual void OnCollision(CollisionData& data);
	inline virtual kepler::Vec2f GetPosition() const override { return m_transforms.front().first; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return m_curDirection; }
	inline virtual kepler::Vec2f GetLastDirection() const override { return m_lastDirection; }
	inline virtual void* GetCollisionInfo() const override { return (void*)&m_bIsAccelarated; }
};