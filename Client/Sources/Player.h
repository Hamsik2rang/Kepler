#pragma once
#pragma once

#include <memory>
#include <functional>

#include "Kepler.h"
#include "GameObject.h"
#include "Animation2D.hpp"
#include "Constant.h"
#include "Collider2D.hpp"

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
	float			m_lastDeltaTime;
	BoxCollider2D*	m_pCollider;
	ePlayerState	m_state;
	bool			m_bIsGrounded;
	bool			m_bIsSpiked;
	std::vector<kepler::KeyCode> m_curInputs;

#ifdef _DEBUG
	kepler::Vec4f m_debugColor;
#endif

	static std::vector<std::shared_ptr<kepler::ITexture2D>> s_pSprites;
	static kepler::AudioSource s_jumpAudio;
	static kepler::AudioSource s_spikeAudio;

	Animation2D m_animation[6];
	Animation2D* m_pCurAnim;	// 현재 상태에 대한 애니메이션을 가리키기만 하는 참조용 변수

	void LoadSprites();
	void ChangeState();
public:
	Player(const kepler::Vec2f& position, const kepler::Vec2f& size, eColliderType type = eColliderType::Box, eColliderCategory category = eColliderCategory::Player);
	virtual ~Player();
	void InitAnimation();
	void OnWin();
	void OnLose();

	virtual void OnEvent(kepler::Event& e);
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender();

	virtual void Respawn();
	// Inherited via GameObject
	virtual void OnCollision(CollisionData& data) override;

	inline virtual kepler::Vec2f GetPosition() const override { return m_position; }
	inline virtual kepler::Vec2f GetSize() const override { return m_size; }
	inline virtual kepler::Vec2f GetCurrentDirection() const override { return m_curDirection; }
	inline virtual kepler::Vec2f GetLastDirection() const override { return m_lastDirection; }
	inline virtual void* GetCollisionInfo() const override { return (void*)&m_bIsSpiked; }
};