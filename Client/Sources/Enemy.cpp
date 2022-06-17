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
	int horizontal = 0;
	int vertical = 0;
	m_bIsSpiked = false;
	m_curInput = 0;

	// TODO: Enemy AI 구현하기
	ComputeBallNextPosition(deltaTime);

	// 예상 위치가 자신 영역일 경우
	if (m_ballNextPosition.x < 0)
	{
		// 공의 x가 자신 영역일 경우
		if (m_pBall->GetPosition().x < 0)
		{
			// 공의 y가 네트의 일정 비율 보다 클 경우
			if (m_pBall->GetPosition().y > (constant::NET_POSITION + constant::NET_SIZE).y / 2)
			{
				// 공x와 자신x가 멀 때
				float distance = GetPosition().x - m_pBall->GetPosition().x;
				distance = distance < 0.0f ? distance * -1.0f : distance;
				if (distance > 50.0f)
				{
					// 공 쪽으로 이동하기
					m_targetX = m_pBall->GetPosition().x - 30.0f;
					MoveToTarget(horizontal);
				}
				else
				{
					// 공 쪽으로 이동하면서 점프하기
					m_targetX = m_pBall->GetPosition().x - 30.0f;
					MoveToTarget(horizontal);
					m_curInput = kepler::key::Up;
					m_bIsSpiked = true;
					vertical += 1;
				}
			}
			else
			{
				// 공x와 자신x가 멀 때
				float distance = GetPosition().x - m_pBall->GetPosition().x;
				distance = distance < 0.0f ? distance * -1.0f : distance;
				if (distance > 50.0f)
				{
					// 공 쪽으로 슬라이딩
					m_targetX = m_pBall->GetPosition().x;
					MoveToTarget(horizontal);
					m_curInput = kepler::key::Space;
					vertical -= 1;
				}
				else
				{
					// 공 쪽으로 이동하면서 점프하기
					m_targetX = m_pBall->GetPosition().x - 30.0f;
					MoveToTarget(horizontal);
					m_curInput = kepler::key::Up;
					m_bIsSpiked = true;
					vertical += 1;
				}
			}
		}
		else
		{
			// 예상 위치의 x쪽으로 이동
			m_targetX = m_ballNextPosition.x;
			MoveToTarget(horizontal);
		}
	}
	else
	{
		// 공 높이가 낮을 수록 뒤로, 높을 수록 앞(가운데)으로 이동하기
		float frontX = m_minX / 2.0f;
		float backX = m_minX + m_size.x;
		float range = frontX - backX;
		m_targetX = backX + (range * m_pBall->GetPosition().y / m_maxY);
		MoveToTarget(horizontal);
	}

	// 목표 위치와 현재 위치의 거리가 m_targetXRange 이하면 좌우 입력 명령을 포기
	float distance = GetPosition().x - m_targetX;
	distance = distance < 0.0f ? distance * -1.0f : distance;
	if (distance <= m_targetXRange)
	{
		horizontal = 0;
	}

	if (m_state != PlayerStateWin && m_state != PlayerStateLose)
	{
		ChangeState(deltaTime, vertical, horizontal);
	}

	// 위치, 방향, 충돌체 및 애니메이션 갱신
	m_position += m_curDirection;
	m_pCollider->SetSize(m_size);
	m_pCollider->SetPosition(m_position);
	m_lastDirection = m_curDirection;
	m_pCurAnim->Update();
}

void Enemy::ComputeBallNextPosition(float deltaTime)
{
	// TODO: 추후 수학 공식으로 대체
	const kepler::Vec2f gravity = { 0, 9.8f };
	kepler::Vec2f curVelocity = m_pBall->GetLastDirection();
	m_ballNextPosition = m_pBall->GetPosition();
	while (m_ballNextPosition.x > m_minX && m_ballNextPosition.x < m_maxX
		&& m_ballNextPosition.y > m_minY && m_ballNextPosition.y < m_maxY)
	{
		curVelocity += gravity;
		// deltaTime은 함수 호출마다 다르기 때문에 예상 값이 실제와 다를 수 있다.
		m_ballNextPosition += curVelocity * deltaTime;
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
		// 네트에 닿은 경우 충돌처리
	case eColliderCategory::Net:
		{
			if (m_curDirection.x > 0.0f)
			{
				m_position.x = -(m_size.x + colliderSize.x) / 2.0f;
			}
		}
		break;
		// 바닥에 닿은 경우 충돌처리. 
		// 이미 바닥과 접해 있는 경우라면 무시하며
		// 점프 후 착지할 경우에만 동작
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
		// 왼쪽 스크린 끝(벽)과 닿은 경우 충돌처리
	case eColliderCategory::Wall:
		{
			if (m_curDirection.x < 0.0f)
			{
				m_position.x = colliderPos.x + (colliderSize.x + m_size.x) / 2.0f;
			}
		}
		break;
	}
}