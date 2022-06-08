#include "Ball.h"

#include "Constant.h"

Ball::Ball(kepler::Vec2f position, kepler::Vec2f size, eColliderType type)
	: m_curDirection{ 0.0f, -1.0f }
	, m_lastDirection{ 0.0f, 0.0f }
	, m_size{ size }
	, m_bIsAccelerated{ false }
	, m_rotation{ false }
	, GameObject(type)
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
	m_rotation += deltaTime * 180.0f;
	if (m_positions.size() > 3)
	{
		m_positions.pop_back();
	}

	m_curDirection = m_lastDirection;
	m_curDirection.y -= 9.8f * deltaTime;

	if (m_bIsAccelerated)
	{
		m_curDirection = { 0.0f, 5.0f };
	}

	kepler::Vec2f nextPosition = m_positions[0] + m_curDirection;
	m_positions.push_front(nextPosition);
	m_lastDirection = m_curDirection;
}

void Ball::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad(m_positions[0], m_rotation, m_size, m_pBallTexture);
	if (m_bIsAccelerated)
	{
		for (int i = 1; i < m_positions.size(); i++)
		{
			kepler::Renderer2D::Get()->DrawQuad(m_positions[i], m_size, m_pBallTexture, false, false, constant::BALL_SHADOW * (0.33f * i));
		}
	}
}