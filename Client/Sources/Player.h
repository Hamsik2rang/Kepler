#pragma once

#include <memory>
#include <functional>

#include "Kepler.h"
#include "GameObject.h"
#include "Animation2D.hpp"

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

	Animation2D m_animation[6];

	Animation2D* m_pCurAnim;
public:
	Player(const kepler::Vec2f& position, const kepler::Vec2f& size, bool bIsJumped, eColliderType colliderType = eColliderType::None);
	void Init();

	void OnEvent(kepler::Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();

	// Inherited via GameObject
	virtual kepler::Vec2f GetPosition() const override { return m_position; }
	virtual kepler::Vec2f GetDirection() const override { return m_curDirection; }
};