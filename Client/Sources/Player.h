#pragma once
#pragma once

#include <memory>
#include <functional>

#include "Kepler.h"
#include "GameObject.h"
#include "Animation2D.hpp"
#include "Constant.h"
#include "CollisionDetector.h"

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
protected:
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
	Player(const kepler::Vec2f& position, const kepler::Vec2f& size, eColliderType type = eColliderType::Box, eColliderCategory category = eColliderCategory::Player);
	void Init();

	virtual void OnEvent(kepler::Event& e);
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender();

	// Inherited via GameObject
	virtual void OnCollision(CollisionData& data) override;

	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return m_curDirection; }
	inline virtual kepler::Vec2f GetLastDirection() const override { return m_lastDirection; }
	inline virtual void GetAdditionalColliderStatus(bool& pOutData) const { pOutData = m_bIsSpiked; }
};