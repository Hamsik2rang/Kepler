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
	// �������� ���� ���� ������ 10�� �̻��� ��� ���� ���� ���� �ϳ� ����
	if (m_transforms.size() > 10)
	{
		m_transforms.pop_back();
	}
	// ���� ���� �״�� �����̵� ���� �������� �߷� ó��
	m_curDirection = m_lastDirection;
	m_curDirection.y -= SPEED_GRAVITY * deltaTime;

	// ���� ���� ������ deque �տ� push
	kepler::Vec2f nextPosition = m_transforms.front().first + m_curDirection;
	m_transforms.push_front(std::make_pair(nextPosition, rotation));
	m_lastDirection = m_curDirection;
	// �浹ü ����(���� ����� �ٲ��� �����Ƿ� �����Ǹ� ������)
	m_pCollider->SetPosition(m_transforms.front().first);
}

void Ball::OnRender()
{
	// ���� ������ ��� �ܻ�鵵 ��ο�
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
	static const float COEF_OF_RES = 0.8f;	// �ݹ߰��(coefficient of restitution)
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
	// collision data �ȿ� �浹ü�� �ΰ������� ��� ��� �ؼ�(���α�豸������ boolŸ�� �ܿ� ���� ���� �����Ƿ� ������ ������ �ؼ� �ʿ� ����)
	if (data.pInfo)
	{
		bIsSpiked = *reinterpret_cast<bool*>(data.pInfo);
	}

	switch (data.pCollider->GetCategory())
	{
		// ���α�� �浹�� ���
	case eColliderCategory::Player:
	case eColliderCategory::Enemy:
		{
			// ���� ���� �ӵ��� ������� ��ݷ� ����
			float impulse = m_curDirection.Length() * SPEED_INPULSE;
			// �� ��ü�� �浹�鿡 ���� �������Ϳ� ��ݷ� ����
			kepler::Vec2f nextDirection = (m_transforms.front().first - colliderPos).Normalize() * (impulse < SPEED_IMPULSE_LIMIT ? impulse : SPEED_IMPULSE_LIMIT);
			// �Ϲ������� Player sprite�� ���ΰ� ���κ��� ª���Ƿ� x�࿡ �����ϰ� ���� �浹���� �� �浹���� y�� ���� �浹�� ����ϵ��� ������.
			nextDirection.x *= 1.5f;

			// ���α�� �ε����� �� ���αⰡ ������ũ ������ �õ��� ���¶�� ���� ó��(�̹� ���� ���̶�� ����) 
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
		// ��Ʈ�� �浹�� ���
	case eColliderCategory::Net:
		{
			// ��Ʈ�� ��� ���α��� �浹 ó���� ����(���α�� ��Ʈ�� �Ѱܼ� �̵��ϸ� �ȵǱ� ������) ȭ�� �߾� ��ü�� ������������ �浹ü ũ�Ⱑ ������
			// ���� ���� ��쿡�� ��Ʈ�� ���� �浹ũ�⺸�� ���� �̵��ϰ� �ִ� ��� �浹ó���� ����Ǹ� �ȵ�
			//
			//					:	<- ���α�� �� �Ѱ� ��Ʈ�� ���� �� �ִ� ������ �ʴ� ��
			//					:	
			//					:	O(��) - ���� ��Ʈ ���� �Ѿ �� �־�� ��
			//					|
			//		(���α�)		|		(���α�) - ���α�� �����ص� ��Ʈ�� �Ѿ �� ����� ��. ���α��� �浹 ó���� �������� ��Ʈ�� ������
			// _______i_________|_________i________
			// �̸� ���� ��Ʈ�� ��� �������� ���� �Ÿ��� �����

			// ��Ʈ�� ��� �������� ���� �Ÿ��� ������ �̻��� ���(��Ʈ�� ��ġ�ų� ���� ���� ���)
			kepler::Vec2f distVector = m_transforms.front().first - (colliderPos + kepler::Vec2f{ 0.0f, constant::NET_SIZE.y / 2.0f });
			if (std::fabsf(distVector.Length()) > m_size.x / 2.0f && m_transforms.front().first.y > (colliderPos.y + constant::NET_SIZE.y / 2.0f))
			{
				break;
			}

			// ��Ʈ�� �浹�ߴٸ� ������ Ǯ���� ��
			m_bIsAccelarated = false;
			// ��Ʈ�� �¿������� ���� �浹�ߴٸ� �����ݻ�(x������)
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
			// ��Ʈ ������� ���� �浹�ߴٸ� ����ݻ�(y������)
			else
			{
				m_curDirection.y *= -1.0f;
				m_transforms[1].first.y = (constant::NET_POSITION.y + (constant::NET_SIZE.y + m_size.y) / 2.0f);
			}
		}
		break;
		// ���� �浹ó��
	case eColliderCategory::Wall:
		{
			m_curDirection.x *= -COEF_OF_RES;
			// ���� ������ ������ Ǯ���� ��.
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
		// ����� �浹 ó��

	case eColliderCategory::Ground:
		{
#ifdef _DEBUG
			m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
			m_bIsGrounded = true;
			m_transforms[1].first.y = constant::GROUND_POSITION.y + (constant::GROUND_SIZE.y + m_size.y) / 2.0f;
			// ���� ���� ��ó���� �ſ� ���� �ӵ��� �����̰� �ִٸ� �ӷ��� �ƿ� ���� ����(����� ���� ƨ��� ����)
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
		// õ��(��ũ�� ���)�� ���� ���
		// ���� �ݻ�(y������)
	case eColliderCategory::Sky:
		{
			m_transforms[1].first.y = constant::SKY_POSITION.y - (constant::SKY_SIZE.y + m_size.y) / 2.0f;
			m_curDirection.y *= -1.0f;
		}
		break;
	}
	// �浹 ó���� ���� ���ŵ� ���� ���� �� �浹ü ����
	m_transforms.front().first = m_transforms[1].first + m_curDirection;
	m_lastDirection = m_curDirection;
	m_pCollider->SetPosition(m_transforms.front().first);
}
