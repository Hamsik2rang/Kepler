#pragma warning(disable: 6812)
#include "Player.h"

#include "CollisionDetector.h"

std::vector<std::shared_ptr<kepler::ITexture2D>> Player::s_pSprites;
kepler::AudioSource Player::s_jumpAudio;
kepler::AudioSource Player::s_spikeAudio;

Player::Player(const kepler::Vec2f& position, const kepler::Vec2f& size, eColliderType type, eColliderCategory category)
	: GameObject(type, category)
	, m_position{ position }
	, m_lastDirection{ -1.0f, 0.0f }
	, m_size{ size }
	, m_bIsGrounded{ false }
	, m_state{ PlayerStateIdle }
	, m_bIsSpiked{ false }
	, m_pCollider{ new BoxCollider2D(*this, position, size, false, category) }
{
	if (s_pSprites.empty())
	{
		LoadSprites();
	}
	InitAnimation();

	if (!s_jumpAudio.GetSound())
	{
		kepler::Audio::Create(s_jumpAudio, "./Assets/Audios/Squritle_kor_jump.mp3");
	}
	if (!s_spikeAudio.GetSound())
	{
		kepler::Audio::Create(s_spikeAudio, "./Assets/Audios/Squritle_kor_spike.mp3");
	}

	CollisionDetector::AddCollider(m_pCollider);
}

Player::~Player()
{
	if (m_pCollider)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}
}

void Player::LoadSprites()
{
	// m_pSprites�� �ִϸ��̼ǿ� ��������Ʈ�� �ҷ��� ������ ���� �����մϴ�.
	// [0]		[1]		[2]		[3]		[4]		[5]		[6]		[7]		[8]
	// | Idle |																  |
	// |        Walk		|												  |
	//						|	  Jump		| Slide |						  |
	//										|		  Lose			|	Win	  |
	//-------------------------------------------------------------------------
	std::string TextureFilePath = "./Assets/Textures/";

	// load and set walk/idle textures
	for (int i = 0; i < 3; i++)
	{
		std::string moveTexturePath = TextureFilePath + "walk" + std::to_string(i + 1) + ".png";
		s_pSprites.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, moveTexturePath));
	}

	// load and set jump texture
	for (int i = 0; i < 2; i++)
	{
		std::string jumpTexturePath = TextureFilePath + "jump" + std::to_string(i + 1) + ".png";
		s_pSprites.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, jumpTexturePath));
	}

	// load and set lose/slide texture
	for (int i = 0; i < 3; i++)
	{
		std::string sildeTexturePath = TextureFilePath + "slide" + std::to_string(i + 1) + ".png";
		s_pSprites.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, sildeTexturePath));
	}

	// load and set win texture
	s_pSprites.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, TextureFilePath + "win.png"));
}

// �ҷ��� Sprite�� �ִϸ��̼� �����ϴ� �Լ�
// Frame Count�� 6�����Ӵ� ��������Ʈ 1�徿 �ѱ⵵�� �����Ͽ����ϴ�.
void Player::InitAnimation()
{
	// set walk animation
	m_animation[PlayerStateWalk].AddSprites({ s_pSprites[0], s_pSprites[1], s_pSprites[2] });
	m_animation[PlayerStateWalk].SetFrameCount(18);
	m_animation[PlayerStateWalk].SetRepeat(true);
	// set idle animation
	m_animation[PlayerStateIdle].AddSprites({ s_pSprites[0] });
	m_animation[PlayerStateIdle].SetFrameCount(4);
	m_animation[PlayerStateIdle].SetRepeat(false);

	// set jump animation
	m_animation[PlayerStateJump].AddSprites({ s_pSprites[3], s_pSprites[4] });
	m_animation[PlayerStateJump].SetFrameCount(12);
	m_animation[PlayerStateJump].SetRepeat(true);

	m_animation[PlayerStateSlide].AddSprites({ s_pSprites[5] });
	m_animation[PlayerStateSlide].SetFrameCount(4);
	m_animation[PlayerStateSlide].SetRepeat(false);

	// load and set lose texture
	m_animation[PlayerStateLose].AddSprites({ s_pSprites[5], s_pSprites[6], s_pSprites[7] });
	m_animation[PlayerStateLose].SetFrameCount(18);
	m_animation[PlayerStateLose].SetRepeat(false);

	// load and set win texture
	m_animation[PlayerStateWin].AddSprites({ s_pSprites[8] });
	m_animation[PlayerStateWin].SetFrameCount(4);
	m_animation[PlayerStateWin].SetRepeat(false);

	m_pCurAnim = &m_animation[PlayerStateIdle];
}

void Player::Respawn()
{
	// ��� �ִϸ��̼� ���� ������ �ʱ�ȭ
	for (int i = 0; i < 6; i++)
	{
		m_animation[i].Start();
	}

	m_bIsGrounded = false;
	m_bIsSpiked = false;

	m_position = constant::PLAYER_SPAWN_POSITION;
	m_size = constant::SQUIRTLE_IDLE_SIZE;
	m_curDirection = { -1.0f, 0.0f };
	m_lastDirection = { 0.0f, 0.0f };
	m_state = PlayerStateIdle;
	m_pCurAnim = &m_animation[PlayerStateIdle];
	m_pCollider->SetPosition(m_position);
	m_pCollider->SetSize(m_size);
}

void Player::ChangeState()
{
	static const float SPEED_JUMP_Y = 1350.0f;
	static const float SPEED_WALK_X = 400.0f;
	static const float SPEED_SLIDE_X = 900.0f;
	static const float SPEED_SLIDE_Y = 400.0f;
	static const float SPEED_GRAVITY = 47.5f;

	int vertical = 0;
	int horizontal = 0;
	bool isSpacePressed = false;

	for (const kepler::KeyCode& input : m_curInputs)
	{
		switch (input)
		{
		case kepler::key::Space:
			isSpacePressed = true;
			break;
		case kepler::key::Up:
			vertical += 1;
			break;
		case kepler::key::Down:
			vertical -= 1;
			break;
		case kepler::key::Right:
			horizontal += 1;
			break;
		case kepler::key::Left:
			horizontal -= 1;
			break;
		}
	}

	// ���� �Ǵ� �����̵� ���°� �ƴ� ���
	if (m_bIsGrounded)
	{
		// ���� ����ִ� ���¿��� ���� ����Ű�� ���� ��� ���� ó��
		if (vertical > 0)
		{
			m_bIsGrounded = false;
			m_state = ePlayerState::PlayerStateJump;
			m_size = constant::SQUIRTLE_JUMP_SIZE;
			if (m_pCurAnim != &m_animation[PlayerStateJump])
			{
				kepler::Audio::Play(s_jumpAudio);
				m_pCurAnim = &m_animation[PlayerStateJump];
				m_pCurAnim->Start();
			}
			m_curDirection = kepler::Vec2f{ horizontal * SPEED_WALK_X, SPEED_JUMP_Y };
		}
		// �Ʒ��� ����Ű�� ���� ���
		else if (vertical < 0 && horizontal != 0)
		{
			// �����̽��ٵ� �Բ� �����ٸ� �ش� ������ ������ �������� �����̵� ó��. �ƴ϶�� ����
			if (isSpacePressed)
			{
				m_bIsGrounded = false;
				m_state = ePlayerState::PlayerStateSlide;
				m_size = constant::SQUIRTLE_SLIDE_SIZE;
				if (m_pCurAnim != &m_animation[PlayerStateSlide])
				{
					m_pCurAnim = &m_animation[PlayerStateSlide];
					m_pCurAnim->Start();
				}
				m_curDirection = kepler::Vec2f{ horizontal * SPEED_SLIDE_X, SPEED_SLIDE_Y };
			}
		}
		// ���� �� �Է��� ������ ���� ���
		else
		{
			m_size = constant::SQUIRTLE_IDLE_SIZE;
			// ������ �Է��� �����ϸ� �ȱ� ó��
			if (horizontal)
			{
				m_state = ePlayerState::PlayerStateWalk;
				if (m_pCurAnim != &m_animation[PlayerStateWalk])
				{
					m_pCurAnim = &m_animation[PlayerStateWalk];
					m_pCurAnim->Start();
				}
				m_curDirection = kepler::Vec2f{ horizontal * SPEED_WALK_X, 0.0f };
			}
			// ������ �Էµ� ���� ��� ������ �� ����(idle)
			else
			{
				m_state = ePlayerState::PlayerStateIdle;
				if (m_pCurAnim != &m_animation[PlayerStateIdle])
				{
					m_pCurAnim = &m_animation[PlayerStateIdle];
					m_pCurAnim->Start();
				}
				m_curDirection = kepler::Vec2f{ 0.0f, 0.0f };
			}
		}
	}
	// ���� �Ǵ� �����̵� ���� ���
	else
	{
		// ���� �������� �߷� ����
		m_curDirection.y = m_lastDirection.y - SPEED_GRAVITY;
		switch (m_state)
		{
		case ePlayerState::PlayerStateSlide:
			{
				// �����̵� ������ ��� �ٴڿ� ������ �� ���� horizontal �� �Է¿� ������ ������ �ȵ�.
				m_curDirection.x = m_lastDirection.x;
			}
			break;
		case ePlayerState::PlayerStateJump:
			{
				// ������ũ ó��
				if (isSpacePressed)
				{
					m_bIsSpiked = true;
					if (!kepler::Audio::IsPlaying(s_spikeAudio))
					{
						kepler::Audio::Play(s_spikeAudio);
					}
				}
				m_curDirection.x = horizontal * 400.0f;
			}
			break;
		}
	}
}

// ���� ���� �� �¸�/�й� ���� ���� �Լ�
void Player::OnWin()
{
	m_curInputs.clear();
	m_state = PlayerStateWin;
	m_size = constant::SQUIRTLE_WIN_SIZE;
	m_pCurAnim = &m_animation[PlayerStateWin];
}

void Player::OnLose()
{
	m_curInputs.clear();
	m_position.y = constant::PLAYER_SPAWN_POSITION.y - 50.0f;
	m_state = PlayerStateLose;
	m_size = constant::SQUIRTLE_LOSE_SIZE;
	m_pCurAnim = &m_animation[PlayerStateLose];
}

void Player::OnEvent(kepler::Event& e)
{

}

// Ű���� ���� ȭ��ǥ ���� �� ����
// Ű���� �Ʒ��� ȭ��ǥ�� �����̽��� ���ÿ� ���� �� �����̵�
// ���� ���¿��� �����̽��� ���� �� ������ũ(������)
void Player::OnUpdate(float deltaTime)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
	int horizontal = 0;
	int vertical = 0;
	m_bIsSpiked = false;
	m_curInputs.clear();

	// ����Ű �Է¿� ���� veritcal, horizontal �� �� ����
	if (kepler::Input::IsButtonDown(kepler::key::Up))
	{
		m_curInputs.push_back(kepler::key::Up);
	}
	if (kepler::Input::IsButtonDown(kepler::key::Down))
	{
		m_curInputs.push_back(kepler::key::Down);
	}
	if (kepler::Input::IsButtonDown(kepler::key::Left))
	{
		m_curInputs.push_back(kepler::key::Left);
	}
	if (kepler::Input::IsButtonDown(kepler::key::Right))
	{
		m_curInputs.push_back(kepler::key::Right);
	}
	if (kepler::Input::IsButtonDown(kepler::key::Space))
	{
		m_curInputs.push_back(kepler::key::Space);
	}

	if (m_state != PlayerStateWin && m_state != PlayerStateLose)
	{
		ChangeState();
	}
	m_lastDeltaTime = deltaTime;
	// ��ġ, ����, �浹ü �� �ִϸ��̼� ����
	m_position += m_curDirection * deltaTime;;
	m_pCollider->SetSize(m_size);
	m_pCollider->SetPosition(m_position);
	m_lastDirection = m_curDirection;
	m_pCurAnim->Update();
}

void Player::OnRender()
{
	// ���� ���°� �ƴ� �� ������ ����Ű�� ������ �ִٸ� ��������Ʈ�� �¿�� ������ ��� ��.
	bool bFlipX = false;
	if (m_state != PlayerStateJump)
	{
		bFlipX = m_curDirection.x > 0.1f;
	}
#ifdef _DEBUG
	if (m_bIsSpiked)
	{
		m_debugColor.g = 1.0f;
}
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX, false, m_debugColor);
#else
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), bFlipX);
#endif
}

void Player::OnCollision(CollisionData& data)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 0.0f, 0.0f, 1.0f };
#endif
	kepler::Vec2f colliderPos = data.pCollider->GetPosition();
	kepler::Vec2f colliderSize = data.pCollider->GetSize();

	switch (data.pCollider->GetCategory())
	{
		// ��Ʈ�� ����� ���� �浹 ó��. ��Ʈ�� ��ġ�� �ʵ��� ��ġ ������
	case eColliderCategory::Net:
		{
			if (m_curDirection.x < 0.0f)
			{
				m_position.x = (m_size.x + colliderSize.x) / 2.0f;
			}
		}
		break;
		// ���� �Ǵ� �����̵� �� ����� ����� ���� �浹 ó��.
		// �̹� ���鿡 ��� �ִ� ���¶�� ������.
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
		// ��(������ ��ũ�� �����ڸ�)�� ����� ��� �浹 ó��
	case eColliderCategory::Wall:
		{
			if (m_curDirection.x > 0.0f)
			{
				m_position.x = colliderPos.x - (colliderSize.x + m_size.x) / 2.0f;
			}
		}
		break;
	}
	// �浹 ó�� ����� ���� ��ġ�� ũ��(�ִϸ��̼��� �ٲ���� �� �����Ƿ�)�� �浹ü�� ����
	m_pCollider->SetPosition(m_position);
	m_pCollider->SetSize(m_size);
}