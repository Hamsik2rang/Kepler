#pragma once

#include <memory>

#include "Kepler.h"

class Player
{
private:
	kepler::Vec2f	m_position{ 100.0f, -200.0f };
	kepler::Vec2f	m_direction{ 0.0f, 0.0f };
	bool			m_bIsJumped = false;

	std::shared_ptr<kepler::ITexture2D> m_pMoveSprite[3];
	std::shared_ptr<kepler::ITexture2D> m_pJumpSprite[2];
	std::shared_ptr<kepler::ITexture2D> m_pSlideSprite[2];
	std::shared_ptr<kepler::ITexture2D> m_pEndGameSprite[2];

	void Move(const kepler::Vec2f position);
	void Jump(const kepler::Vec2f jump);
public:

	void Init(bool bIsEnemy);

	void Update(const float vertical, const float horizontal, bool bIsJumped);
	void OnUpdate(const float deltaTime);
	void OnRender();
};