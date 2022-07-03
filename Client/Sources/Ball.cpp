#include "Ball.h"

#include "GameLayer.h"
#include "Constant.h"
#include "CollisionDetector.h"

Ball::Ball(kepler::Vec2f position, float radius, eColliderType type, eColliderCategory category)
	: m_curDirection{ 0.0f, -1.0f }
	, m_lastDirection{ 0.0f, 0.0f }
	, m_size{ radius, radius }
	, m_deltaTime{ 0.0f }
	, m_bIsAccelarated{ false }
	, m_bIsGrounded{ false }
	, GameObject(type, category)
	, m_pCollider{ new CircleCollider2D(*this, position, radius, false, category) }
{
	m_transforms.push_front(std::make_pair(position, 0.0f));
	CollisionDetector::AddCollider(m_pCollider);
	InitSprite();
}

Ball::~Ball()
{
	if (m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}
}

void Ball::InitSprite()
{
	m_pBallSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/ball.png");
	m_pImpactSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/impact.png");
}

void Ball::Respawn(bool bSpawnAbovePlayer)
{
	m_transforms.clear();
	if (bSpawnAbovePlayer)
	{
		m_transforms.push_front(std::make_pair(constant::BALL_PLAYER_SPAWN_POSITION, 0.0f));
	}
	else
	{
		m_transforms.push_front(std::make_pair(constant::BALL_ENEMY_SPAWN_POSITION, 0.0f));
	}
	m_curDirection = kepler::Vec2f::Zero;
	m_lastDirection = kepler::Vec2f::Zero;
	m_pCollider->SetPosition(m_transforms.front().first);
}

void Ball::OnEvent(kepler::Event& e)
{

}

void Ball::OnUpdate(float deltaTime)
{
	static const float SPEED_GRAVITY = 9.8f;

	m_deltaTime = deltaTime;

#ifdef _DEBUG
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
	float rotation = m_transforms.front().second + deltaTime * 180.0f;
	// 저장중인 공의 이전 위상이 10개 이상인 경우 가장 예전 위상 하나 삭제
	if (m_transforms.size() > 10)
	{
		m_transforms.pop_back();
	}
	// 직전 방향 그대로 움직이되 연직 방향으로 중력 처리
	m_curDirection = m_lastDirection;
	m_curDirection.y -= SPEED_GRAVITY * deltaTime;

	// 새로 계산된 위상을 deque 앞에 push
	kepler::Vec2f nextPosition = m_transforms.front().first + m_curDirection;
	m_transforms.push_front(std::make_pair(nextPosition, rotation));
	m_lastDirection = m_curDirection;
	// 충돌체 갱신(공은 사이즈가 바뀌지 않으므로 포지션만 갱신함)
	m_pCollider->SetPosition(m_transforms.front().first);
}

void Ball::OnRender()
{
	// 가속 상태인 경우 잔상들도 드로우
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
	static const float COEF_OF_RES = 0.8f;	// 반발계수(coefficient of restitution)
	static const float SPEED_ACCELERATION = 2000.0f;
	static const float SPEED_INPULSE = 120.0f;
	static const float SPEED_IMPULSE_LIMIT = 600.0f;

#ifdef _DEBUG
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif
	kepler::Vec2f colliderPos = data.pCollider->GetPosition();
	kepler::Vec2f colliderDir = data.pCollider->GetDirection();
	kepler::Vec2f colliderSize = data.pCollider->GetSize();
	bool bIsSpiked = false;
	// collision data 안에 충돌체의 부가정보가 담긴 경우 해석(꼬부기배구에서는 bool타입 외에 받을 일이 없으므로 별도의 데이터 해석 필요 없음)
	if (data.pInfo)
	{
		bIsSpiked = *reinterpret_cast<bool*>(data.pInfo);
	}

	switch (data.pCollider->GetCategory())
	{
		// 꼬부기와 충돌한 경우
	case eColliderCategory::Player:
	case eColliderCategory::Enemy:
		{
			// 현재 공의 속도를 기반으로 충격량 설정
			float impulse = m_curDirection.Length() * SPEED_INPULSE;
			// 두 물체의 충돌면에 대한 법선벡터에 충격량 제한
			kepler::Vec2f nextDirection = (m_transforms.front().first - colliderPos).Normalize() * (impulse < SPEED_IMPULSE_LIMIT ? impulse : SPEED_IMPULSE_LIMIT);
			// 일반적으로 Player sprite의 가로가 세로보다 짧으므로 x축에 평행하게 공이 충돌했을 때 충돌량을 y축 평행 충돌과 비슷하도록 보정함.
			nextDirection.x *= 1.5f;

			// 꼬부기와 부딛혔을 때 꼬부기가 스파이크 공격을 시도한 상태라면 가속 처리(이미 가속 중이라면 유지) 
			if (bIsSpiked)
			{
				nextDirection = nextDirection.Normalize() * SPEED_ACCELERATION;
				m_bIsAccelarated = true;
			}
			else
			{
				m_bIsAccelarated = false;
			}
			nextDirection *= m_deltaTime;
			m_curDirection = nextDirection;
		}
		break;
		// 네트와 충돌한 경우
	case eColliderCategory::Net:
		{
			// 네트의 경우 꼬부기의 충돌 처리를 위해(꼬부기는 네트를 넘겨서 이동하면 안되기 때문에) 화면 중앙 전체를 가로지르도록 충돌체 크기가 설정됨
			// 따라서 공의 경우에는 네트의 실제 충돌크기보다 위로 이동하고 있는 경우 충돌처리가 진행되면 안됨
			//
			//					:	<- 꼬부기는 못 넘고 네트는 넘을 수 있는 보이지 않는 벽
			//					:	
			//					:	O(공) - 공은 네트 위를 넘어갈 수 있어야 함
			//					|
			//		(꼬부기)		|		(꼬부기) - 꼬부기는 점프해도 네트를 넘어갈 수 없어야 함. 꼬부기의 충돌 처리를 기준으로 네트가 설정됨
			// _______i_________|_________i________
			// 이를 위해 네트의 상단 꼭지점과 공의 거리를 계산함

			// 네트의 상단 꼭지점과 공의 거리가 반지름 이상인 경우(네트와 걸치거나 닿지 않은 경우)
			kepler::Vec2f distVector = m_transforms.front().first - (colliderPos + kepler::Vec2f{ 0.0f, constant::NET_SIZE.y / 2.0f });
			if (std::fabsf(distVector.Length()) > m_size.x / 2.0f && m_transforms.front().first.y > (colliderPos.y + constant::NET_SIZE.y / 2.0f))
			{
				break;
			}

			// 네트와 충돌했다면 가속이 풀려야 함
			m_bIsAccelarated = false;
			// 네트의 좌우측으로 공이 충돌했다면 수직반사(x값반전)
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
			// 네트 상단으로 공이 충돌했다면 수평반사(y값반전)
			else
			{
				m_curDirection.y *= -1.0f;
				m_transforms[1].first.y = (constant::NET_POSITION.y + (constant::NET_SIZE.y + m_size.y) / 2.0f);
			}
		}
		break;
		// 벽과 충돌처리
	case eColliderCategory::Wall:
		{
			m_curDirection.x *= -COEF_OF_RES;
			// 벽과 닿으면 가속이 풀려야 함.
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
		// 지면과 충돌 처리

	case eColliderCategory::Ground:
		{
#ifdef _DEBUG
			m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
			m_bIsGrounded = true;
			m_transforms[1].first.y = constant::GROUND_POSITION.y + (constant::GROUND_SIZE.y + m_size.y) / 2.0f;
			// 공이 지면 근처에서 매우 작은 속도로 움직이고 있다면 속력을 아예 없애 버림(지면과 무한 튕기기 방지)
			if (std::fabsf(m_curDirection.y) < 0.5f)
			{
				m_curDirection.y = 0.0f;
			}
			else
			{
				m_curDirection = kepler::Vec2f{ m_curDirection.x, -m_curDirection.y } * COEF_OF_RES;
			}
			m_bIsAccelarated = false;
		}
		break;
		// 천장(스크린 상단)과 닿은 경우
		// 수평 반사(y값반전)
	case eColliderCategory::Sky:
		{
			m_transforms[1].first.y = constant::SKY_POSITION.y - (constant::SKY_SIZE.y + m_size.y) / 2.0f;
			m_curDirection.y *= -1.0f;
		}
		break;
	}
	// 충돌 처리에 의해 갱신된 위상 지정 및 충돌체 갱신
	m_transforms.front().first = m_transforms[1].first + m_curDirection;
	m_lastDirection = m_curDirection;
	m_pCollider->SetPosition(m_transforms.front().first);
}
