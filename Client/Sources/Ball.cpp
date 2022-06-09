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

#ifdef _DEBUG
	if (kepler::Input::IsButtonDown(kepler::key::R))
	{
		m_positions[0] = constant::BALL_PLAYER_SPAWN_POSITION;
		m_curDirection = { 0.0f, -1.0f };
	}
#endif

	kepler::Vec2f nextPosition = m_positions[0] + m_curDirection;
	m_positions.push_front(nextPosition);
	m_lastDirection = m_curDirection;
}

void Ball::OnRender()
{
#ifdef _DEBUG
	kepler::Renderer2D::Get()->DrawQuad(m_positions[0], m_rotation, m_size, m_pBallTexture, false, false, m_debugColor);
#else
	{
		kepler::Renderer2D::Get()->DrawQuad(m_positions[0], m_rotation, m_size, m_pBallTexture);
#endif
		if (m_bIsAccelarated)
		{
			for (int i = 1; i < m_positions.size(); i++)
			{
				kepler::Renderer2D::Get()->DrawQuad(m_positions[i], m_size, m_pBallTexture, false, false, constant::BALL_SHADOW * (0.33f * i));
			}
		}
}

	void Ball::OnCollision(CollisionData & data)
	{
		static float COEF_OF_RES = 0.8f;

		m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		kepler::Vec2f colliderPos = data.collider->GetPosition();
		kepler::Vec2f colliderDir = data.collider->GetCurrentDirection();
		kepler::Vec2f colliderSize = data.collider->GetSize();
		bool bIsSpiked = data.bIsSpiked;

		switch (data.collider->GetColliderCategory())
		{
		case eColliderCategory::Player:
		case eColliderCategory::Enemy:
			{
				m_debugColor = { 0.0f, 0.3f, 1.0f, 1.0f };
				float impulse = m_curDirection.Length();
				// 두 물체의 충돌면에 대한 법선벡터에 충격량 제한
				kepler::Vec2f nextDirection = (m_positions[0] - colliderPos).Normalize() * (impulse < 10.0f ? impulse : 10.0f);
				// Player sprite의 가로가 세로보다 짧으므로 x축에 평행하게 공이 충돌했을 때 충돌량을 y축 평행 충돌과 비슷하도록 보정함.
				nextDirection.x *= 1.5f;
				kepler::Vec2f xAxis = kepler::Vec2f::Right;

				float cosAngle = kepler::Dot(xAxis, m_curDirection.Normalize());
				if (std::fabsf(cosAngle) > std::cosf(kepler::math::constant::PIDIV4))
				{
					m_curDirection.y *= -1.0f;
				}
				else
				{
					m_curDirection.x *= -1.0f;
				}
				if (data.bIsSpiked)
				{
					m_bIsAccelarated = true;
					m_curDirection *= 5.0f;
				}
				m_curDirection = nextDirection;
			}
			break;
		case eColliderCategory::Net:
			{
				kepler::Vec2f distVector = m_positions[0] - (colliderPos + kepler::Vec2f{ 0.0f, constant::NET_SIZE.y / 2.0f });

				if (std::fabsf(distVector.Length()) < m_size.x / 2.0f || m_positions[0].y < (colliderPos.y + constant::NET_SIZE.y / 2.0f))
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
				m_curDirection = { m_curDirection.x, -m_curDirection.y * COEF_OF_RES };

				// TODO: set end
				// call gamemanager
			}
			break;
		}
		m_positions[0] = m_positions[1] + m_curDirection;
		m_lastDirection = m_curDirection;
	}
