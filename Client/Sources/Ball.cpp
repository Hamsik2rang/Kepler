#include "Ball.h"

#include "Constant.h"
#include "CollisionDetector.h"

Ball::Ball(kepler::Vec2f position, float radius, eColliderType type, eColliderCategory category)
	: m_curDirection{ 0.0f, -1.0f }
	, m_lastDirection{ 0.0f, 0.0f }
	, m_size{ radius, radius }
	, m_bIsAccelarated{ false }
	, m_bIsGrounded{ false }
	, GameObject(type, category)
	, m_pCollider{ new CircleCollider2D(*this, position, radius, false, category) }
{
	m_transforms.push_front(std::make_pair(position, 0.0f));
	CollisionDetector::AddCollider(m_pCollider);
	Init();
}

Ball::~Ball()
{
	if (m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}
}

void Ball::Init()
{
	m_pBallSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/ball.png");
	m_pImpactSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/impact.png");
}

void Ball::OnEvent(kepler::Event& e)
{

}

void Ball::OnUpdate(float deltaTime)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
	float rotation = m_transforms.front().second + deltaTime * 180.0f;
	if (m_transforms.size() > 10)
	{
		m_transforms.pop_back();
	}

	m_curDirection = m_lastDirection;
	m_curDirection.y -= 9.8f * deltaTime;

#ifdef _DEBUG
	if (kepler::Input::IsButtonDown(kepler::key::R))
	{
		m_transforms.front().first = constant::BALL_PLAYER_SPAWN_POSITION;
		m_curDirection = { 0.0f, -1.0f };
	}
#endif

	kepler::Vec2f nextPosition = m_transforms.front().first + m_curDirection;
	m_transforms.push_front(std::make_pair(nextPosition, rotation));
	m_lastDirection = m_curDirection;
	m_pCollider->SetPosition(m_transforms.front().first);
}

void Ball::OnRender()
{
	if (m_bIsAccelarated)
	{
		for (int i = 1; i < m_transforms.size(); i++)
		{
			kepler::Renderer2D::Get()->DrawQuad(m_transforms[i].first, m_transforms[i].second, m_size, m_pBallSprite, false, false, constant::BALL_SHADOW * (0.1f * i));
		}
	}
#ifdef _DEBUG
	kepler::Renderer2D::Get()->DrawQuad(m_transforms.front().first, m_transforms.front().second, m_size, m_pBallSprite, false, false, m_debugColor);
#else
	kepler::Renderer2D::Get()->DrawQuad(m_transforms.front().first, m_transforms.front().second, m_size, m_pBallSprite);
#endif
	if (m_bIsGrounded)
	{
		kepler::Renderer2D::Get()->DrawQuad(m_transforms.front().first - kepler::Vec2f{ 0.0f, m_size.y / 2.0f + 20.0f }, m_size * 2.0f, m_pImpactSprite);
		m_bIsGrounded = false;
	}
}

void Ball::OnCollision(CollisionData& data)
{
	static float COEF_OF_RES = 0.8f;

#ifdef _DEBUG
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif
	kepler::Vec2f colliderPos = data.collider->GetPosition();
	kepler::Vec2f colliderDir = data.collider->GetDirection();
	kepler::Vec2f colliderSize = data.collider->GetSize();
	bool bIsSpiked = false;
	if (data.bIsSpiked)
	{
		bIsSpiked = *reinterpret_cast<bool*>(data.bIsSpiked);
	}

	switch (data.collider->GetCategory())
	{
	case eColliderCategory::Player:
	case eColliderCategory::Enemy:
		{
			float impulse = m_curDirection.Length();
			// 두 물체의 충돌면에 대한 법선벡터에 충격량 제한
			kepler::Vec2f nextDirection = (m_transforms.front().first - colliderPos).Normalize() * (impulse < 10.0f ? impulse : 10.0f);
			// Player sprite의 가로가 세로보다 짧으므로 x축에 평행하게 공이 충돌했을 때 충돌량을 y축 평행 충돌과 비슷하도록 보정함.
			nextDirection.x *= 1.5f;

			if (bIsSpiked)
			{
				nextDirection = nextDirection.Normalize() * 35.0f;
				m_bIsAccelarated = true;
			}
			else
			{
				m_bIsAccelarated = false;
			}
			m_curDirection = nextDirection;
		}
		break;
	case eColliderCategory::Net:
		{
			kepler::Vec2f distVector = m_transforms.front().first - (colliderPos + kepler::Vec2f{ 0.0f, constant::NET_SIZE.y / 2.0f });
			if (std::fabsf(distVector.Length()) > m_size.x / 2.0f && m_transforms.front().first.y > (colliderPos.y + constant::NET_SIZE.y / 2.0f))
			{
				break;
			}

			m_bIsAccelarated = false;
			if (m_transforms.front().first.y < colliderPos.y + constant::NET_SIZE.y / 2.0f)
			{
				m_curDirection.x *= -1.0f;
				if (m_transforms.front().first.x < 0.0f)
				{
					m_transforms[1].first.x = -(constant::NET_SIZE.x + m_size.x) / 2.0f;
				}
				else
				{
					m_transforms[1].first.x = (constant::NET_SIZE.x + m_size.x) / 2.0f;
				}
			}
			else
			{
				m_curDirection.y *= -1.0f;
				m_transforms[1].first.y = (constant::NET_POSITION.y + (constant::NET_SIZE.y + m_size.y) / 2.0f);
			}

		}
		break;
	case eColliderCategory::Wall:
		{
			m_curDirection.x *= -COEF_OF_RES;
			m_bIsAccelarated = false;
			if (colliderPos.x < 0.0f)
			{
				m_transforms[1].first.x = colliderPos.x + (colliderSize.x + m_size.x) / 2.0f;
			}
			else
			{
				m_transforms[1].first.x = colliderPos.x - (colliderSize.x + m_size.x) / 2.0f;
			}
		}
		break;
	case eColliderCategory::Ground:
		{
#ifdef _DEBUG
			m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
			m_bIsGrounded = true;
			m_transforms[1].first.y = constant::GROUND_POSITION.y + (constant::GROUND_SIZE.y + m_size.y) / 2.0f;
			if (std::fabsf(m_curDirection.y) < 0.5f)
			{
				m_curDirection.y = 0.0f;
			}
			else
			{
				m_curDirection = kepler::Vec2f{ m_curDirection.x, -m_curDirection.y } *COEF_OF_RES;
			}
			m_bIsAccelarated = false;

			// TODO: set end
			// call gamemanager

		}
		break;
	case eColliderCategory::Sky:
		{
			m_transforms[1].first.y = constant::SKY_POSITION.y - (constant::SKY_SIZE.y + m_size.y) / 2.0f;
			m_curDirection.y *= -1.0f;
		}
		break;
	}
	m_transforms.front().first = m_transforms[1].first + m_curDirection;
	m_lastDirection = m_curDirection;
	m_pCollider->SetPosition(m_transforms.front().first);
}
