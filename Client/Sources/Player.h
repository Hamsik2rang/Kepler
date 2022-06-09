#pragma once
#pragma once

#include <memory>
#include <functional>

#include "Kepler.h"
#include "GameObject.h"
#include "Animation2D.hpp"
#include "Constant.h"

enum ePlayerState
{
	PlayerStateIdle = 0,
	PlayerStateWalk,
	PlayerStateJump,
	PlayerStateSlide,
	PlayerStateWin,
	PlayerStateLose
};

class Player : public GameObject
{
private:
	kepler::Vec2f	m_position;
	kepler::Vec2f	m_size;
	kepler::Vec2f	m_curDirection;
	kepler::Vec2f	m_lastDirection;
	ePlayerState	m_state;
	bool			m_bIsGrounded;
	bool			m_bIsSpiked;

#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

	Animation2D m_animation[6];
	Animation2D* m_pCurAnim;

public:
	Player(const kepler::Vec2f& position, const kepler::Vec2f& size, bool bIsJumped, eColliderType type = eColliderType::None, eColliderCategory category = eColliderCategory::Player);
	void Init();

	void OnEvent(kepler::Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();

	// Inherited via GameObject
	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return m_curDirection; }
	inline virtual kepler::Vec2f GetLastDirection() const override { return m_lastDirection; }
	virtual void GetCollisionData(void* pOutData) const { pOutData = (void*)&m_bIsSpiked; }
	virtual void OnCollision(CollisionData& data) override;
};