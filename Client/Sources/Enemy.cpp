#include "Enemy.h"

#include "CollisionDetector.h"
#include "Constant.h"

Enemy::Enemy(const kepler::Vec2f& position, const kepler::Vec2f& size, bool bIsJumped, eColliderType type, eColliderCategory category)
	: Player(position, size, bIsJumped, type, category)
{

}

void Enemy::OnEvent(kepler::Event& e)
{

}

void Enemy::OnUpdate(float deltaTime)
{
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void Enemy::OnRender()
{
	bool bFlipX = true;
	if (m_state != PlayerStateJump)
	{
		bFlipX = m_curDirection.x > 0.0f;
	}
#ifdef _DEBUG
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX, false, m_debugColor);
#else
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX);
#endif
}

void Enemy::OnCollision(CollisionData& data)
{
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	kepler::Vec2f colliderPos = data.collider->GetPosition();
	kepler::Vec2f colliderSize = data.collider->GetSize();

	switch (data.collider->GetColliderCategory())
	{
	case eColliderCategory::Ball:
		{

		}
		break;
	case eColliderCategory::Ground:
		{

		}
		break;
	case eColliderCategory::Net:
		{

		}
		break;
	case eColliderCategory::Wall:
		{

		}
		break;
	}
}
