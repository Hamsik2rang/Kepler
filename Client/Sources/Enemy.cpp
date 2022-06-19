#include "Enemy.h"

#include "CollisionDetector.h"
#include "Constant.h"

Enemy::Enemy(const kepler::Vec2f& position, const kepler::Vec2f& size, std::shared_ptr<Level> pLevel, std::shared_ptr<Player> pPlayer, std::shared_ptr<Ball> pBall, eColliderType type, eColliderCategory category)
	: Player(position, size, type, category)
{
	InitSprite();
	m_pLevel = pLevel;
	m_pPlayer = pPlayer;
	m_pBall = pBall; 

	m_minX = m_pLevel->GetWidth() / -2.0f;
	m_maxX = m_pLevel->GetWidth() / 2.0f;
	m_minY = m_pLevel->GetHeight() / -2.0f;
	m_maxY = m_pLevel->GetHeight() / 2.0f;

	m_computeTimer.Init();
}

void Enemy::Respawn()
{
	m_bIsGrounded = false;
	m_bIsSpiked = false;
	m_position = { -constant::PLAYER_SPAWN_POSITION.x, constant::PLAYER_SPAWN_POSITION.y };
	m_size = constant::SQUIRTLE_IDLE_SIZE;
	m_curDirection = { 1.0f, 0.0f };
	m_lastDirection = { 0.0f, 0.0f };
	m_state = PlayerStateIdle;
	m_pCurAnim = &m_animation[PlayerStateIdle];
	m_pCollider->SetPosition(m_position);
	m_pCollider->SetSize(m_size);
}

void Enemy::ChangeState(float deltaTime, int vertical, int horizontal)
{
	Player::ChangeState(deltaTime, vertical, horizontal);
}

void Enemy::OnEvent(kepler::Event& e)
{

}

void Enemy::OnUpdate(float deltaTime)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
	// update AI State
	// m_horizontal = 0; ��, �� �Է��� ���� �� �����ϱ�
	m_vertical = 0;
	m_bIsSpiked = false;
	m_curInput = 0;

	// TODO: Enemy AI �����ϱ�
	// ���� �ð� ���� ��Ȳ �Ǵ��ϱ�
	if (m_computeTimer.Elapsed() >= 0.3f)
	{
		m_computeTimer.Start();
		ComputeBallNextPosition(deltaTime);
		Logic();
	}

	if (m_state != PlayerStateWin && m_state != PlayerStateLose)
	{
		ChangeState(deltaTime, m_vertical, m_horizontal);
	}

	// ��ġ, ����, �浹ü �� �ִϸ��̼� ����
	m_position += m_curDirection;
	m_pCollider->SetSize(m_size);
	m_pCollider->SetPosition(m_position);
	m_lastDirection = m_curDirection;
	m_pCurAnim->Update();
}

void Enemy::ComputeBallNextPosition(float deltaTime)
{
	// TODO: ���� ���� �������� ��ü
	const kepler::Vec2f gravity = { 0, -9.8f };
	kepler::Vec2f curVelocity = m_pBall->GetLastDirection();
	m_ballNextPosition = m_pBall->GetPosition();
	while (m_ballNextPosition.x > m_minX && m_ballNextPosition.x < m_maxX
		&& m_ballNextPosition.y > m_minY && m_ballNextPosition.y < m_maxY)
	{
		curVelocity += gravity * deltaTime;
		// deltaTime�� �Լ� ȣ�⸶�� �ٸ��� ������ ���� ���� ������ �ٸ� �� �ִ�.
		m_ballNextPosition += curVelocity;
	}
}

bool Enemy::MoveToTarget(int& outHorizontal)
{
	if (!m_bIsGrounded)
	{
		return false;
	}

	bool bIsLeft = m_targetX < GetPosition().x;
	m_curInput = bIsLeft ? kepler::key::Left : kepler::key::Right;
	outHorizontal = bIsLeft ? -1 : 1;

	return true;
}

void Enemy::Logic()
{
	// ���� ��ġ�� �ڽ� ������ ���
	if (m_ballNextPosition.x < 0)
	{
		// ���� x�� �ڽ� ������ ���
		if (m_pBall->GetPosition().x < 0)
		{
			// ���� y�� ��Ʈ�� ���� ���� ���� Ŭ ���
			if (m_pBall->GetPosition().y > constant::NET_POSITION.y + constant::NET_SIZE.y / 2)
			{
				// ��x�� �ڽ�x�� �� ��
				float distance = GetPosition().x - m_pBall->GetPosition().x;
				distance = distance < 0.0f ? distance * -1.0f : distance;
				if (distance > 50.0f)
				{
					// �� ������ �̵��ϱ�
					m_targetX = m_pBall->GetPosition().x - m_pBall->GetSize().x;
					MoveToTarget(m_horizontal);
				}
				else
				{
					// �� ������ �̵��ϸ鼭 �����ϱ�
					m_targetX = m_pBall->GetPosition().x - m_pBall->GetSize().x;
					MoveToTarget(m_horizontal);
					m_curInput = kepler::key::Up;
					m_bIsSpiked = true;
					m_vertical = 1;
				}
			}
			else
			{
				// ��x�� �ڽ�x�� �� ��
				float distance = GetPosition().x - m_pBall->GetPosition().x;
				distance = distance < 0.0f ? distance * -1.0f : distance;
				if (distance > 50.0f)
				{
					// �� ������ �����̵�
					m_targetX = m_pBall->GetPosition().x;
					MoveToTarget(m_horizontal);
					m_curInput = kepler::key::Space;
					m_vertical = -1;
				}
				else
				{
					// �� ������ �̵��ϸ鼭 �����ϱ�
					m_targetX = m_pBall->GetPosition().x - m_pBall->GetSize().x;
					MoveToTarget(m_horizontal);
					m_curInput = kepler::key::Up;
					m_bIsSpiked = true;
					m_vertical = 1;
				}
			}
		}
		else
		{
			// ���� ��ġ�� x������ �̵�
			m_targetX = m_ballNextPosition.x;
			MoveToTarget(m_horizontal);
		}
	}
	else
	{
		// �� ���̰� ���� ���� �ڷ�, ���� ���� ��(���)���� �̵��ϱ�
		float frontX = m_minX / 2.0f;
		float backX = m_minX + m_size.x;
		float range = frontX - backX;
		m_targetX = backX + (range * m_pBall->GetPosition().y / m_maxY);
		MoveToTarget(m_horizontal);
	}

	// ��ǥ ��ġ�� ���� ��ġ�� �Ÿ��� m_targetXRange ���ϸ� �¿� �Է� ����� ����
	float distance = GetPosition().x - m_targetX;
	distance = distance < 0.0f ? distance * -1.0f : distance;
	if (distance <= m_targetXRange)
	{
		m_horizontal = 0;
	}
}

void Enemy::OnRender()
{
	bool bFlipX = true;
	if (m_state != PlayerStateJump)
	{
		bFlipX = m_curDirection.x > -0.1f;
	}
#ifdef _DEBUG
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX, false, m_debugColor);
#else
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX);
#endif
}

void Enemy::OnCollision(CollisionData& data)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif
	kepler::Vec2f colliderPos = data.pCollider->GetPosition();
	kepler::Vec2f colliderSize = data.pCollider->GetSize();

	switch (data.pCollider->GetCategory())
	{
		// ��Ʈ�� ���� ��� �浹ó��
	case eColliderCategory::Net:
		{
			if (m_curDirection.x > 0.0f)
			{
				m_position.x = -(m_size.x + colliderSize.x) / 2.0f;
			}
		}
		break;
		// �ٴڿ� ���� ��� �浹ó��. 
		// �̹� �ٴڰ� ���� �ִ� ����� �����ϸ�
		// ���� �� ������ ��쿡�� ����
	case eColliderCategory::Ground:
		{
#ifdef _DEBUG
			m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
			if (!m_bIsGrounded)
			{
				m_bIsGrounded = true;
				m_bIsSpiked = false;
				m_curDirection = { 0.0f, 0.0f };
				m_size = constant::SQUIRTLE_IDLE_SIZE;
				m_position.y = constant::GROUND_POSITION.y + m_size.y / 2.0f;

				m_state = ePlayerState::PlayerStateIdle;
				m_pCurAnim = &m_animation[PlayerStateIdle];
				m_pCurAnim->Start();
			}
		}
		break;
		// ���� ��ũ�� ��(��)�� ���� ��� �浹ó��
	case eColliderCategory::Wall:
		{
			if (m_curDirection.x < 0.0f)
			{
				m_position.x = colliderPos.x + (colliderSize.x + m_size.x) / 2.0f;
			}
		}
		break;
		// ���� ����� �� �浹ó��
	case eColliderCategory::Ball:
		{

		}
		break;
	}
}