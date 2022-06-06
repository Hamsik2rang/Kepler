#pragma once

#include <memory>
#include <functional>

#include "Kepler.h"
#include "GameObject.h"

class Player : public GameObject
{
private:
	std::function<void(std::shared_ptr<kepler::ITexture2D>*)>  m_animationCallback;

	kepler::Vec2f	m_position;
	kepler::Vec2f	m_size;
	kepler::Vec2f	m_direction;
	bool			m_bIsGrounded;
	bool			m_bIsSpiked;

	std::shared_ptr<kepler::ITexture2D> m_pMoveSprite[3];
	std::shared_ptr<kepler::ITexture2D> m_pJumpSprite[2];
	std::shared_ptr<kepler::ITexture2D> m_pSlideSprite[2];
	std::shared_ptr<kepler::ITexture2D> m_pEndGameSprite[2];

	std::shared_ptr<kepler::ITexture2D> m_nextRenderSprite;

public:
	Player(kepler::Vec2f& position, kepler::Vec2f& size, bool bIsJumped, eColliderType colliderType = eColliderType::None);
	void Init();

	void OnEvent(kepler::Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();
};