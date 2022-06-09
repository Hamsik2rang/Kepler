#include "Ball.h"

#include "Constant.h"
#include "CollisionDetector.h"

Ball::Ball(kepler::Vec2f position, float radius, eColliderType type, eColliderCategory category)
	: m_curDirection{ 0.0f, -1.0f }
	, m_lastDirection{ 0.0f, 0.0f }
	, m_size{ radius, radius }
	, m_bIsAccelarated{ false }
	, m_bIsGrounded{ false }
	, m_rotation{ false }
	, GameObject(type, category)
{
	m_positions.push_front(position);
	Init();
}

void Ball::Init()
{
	m_pBallTexture = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/ball.png");
	m_pImpactTexture = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/impact.png");
}

void Ball::OnEvent(kepler::Event& e)
{

}

void Ball::OnUpdate(float deltaTime)
{
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_rotation += deltaTime * 180.0f;
	if (m_positions.size() > 3)
	{
		m_positions.pop_back();
	}

	m_curDirection = m_lastDirection;
	m_curDirection.y -= 9.8f * deltaTime;

	if (m_bIsAccelarated)
	{
		m_curDirection = { 0.0f, 5.0f };
	}

	kepler::Vec2f nextPosition = m_positions[0] + m_curDirection;
	m_positions.push_front(nextPosition);
	m_lastDirection = m_curDirection;
}

void Ball::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad(m_positions[0], m_rotation, m_size, m_pBallTexture, false, false, m_debugColor);
	if (m_bIsAccelarated)
	{
		for (int i = 1; i < m_positions.size(); i++)
		{
			kepler::Renderer2D::Get()->DrawQuad(m_positions[i], m_size, m_pBallTexture, false, false, constant::BALL_SHADOW * (0.33f * i));
		}
	}
}

void Ball::OnCollision(CollisionData& data)
{
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	kepler::Vec2f colliderPos = data.collider->GetPosition();
	kepler::Vec2f colliderSize = data.collider->GetSize();
	bool bIsSpiked = data.bIsSpiked;

	switch (data.collider->GetColliderCategory())
	{
	case eColliderCategory::Player:
		{
			m_curDirection.y *= -1.0f;
		}
		break;
	case eColliderCategory::Enemy:
		{

		}
		break;
	case eColliderCategory::Net:
		{
			if (m_positions[0].y - m_size.y - (colliderPos.y + colliderSize.y) > kepler::math::constant::EPSILON)
			{
				m_curDirection.y *= -1.0f;
			}
			else
			{
				m_curDirection.x *= -1.0f;
			}
		}
		break;
	case eColliderCategory::Wall:
		{
			m_curDirection.x *= -1.0f;
		}
		break;
	case eColliderCategory::Ground:
		{
			m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_bIsGrounded = true;
			m_positions[0].y = constant::GROUND_POSITION.y + m_size.y / 2.0f;
			m_curDirection = { 0.0f, 0.0f };
			m_lastDirection = { 0.0f, 0.0f };
			m_rotation = 0.0f;
			// end set
			// call gamemanager
		}
		break;
	}
}
