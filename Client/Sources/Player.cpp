#pragma warning(disable: 6812)
#include "Player.h"

#include "CollisionDetector.h"

Player::Player(const kepler::Vec2f& position, const kepler::Vec2f& size, eColliderType type, eColliderCategory category)
	: GameObject(type, category)
	, m_position{ position }
	, m_lastDirection{ -1.0f, 0.0f }
	, m_size{ size }
	, m_bIsGrounded{ false }
	, m_state{ ePlayerState::PlayerStateIdle }
	, m_bIsSpiked{ false }
	, m_pCollider{ new BoxCollider2D(*this, position, size, false, category) }
{
	InitSprite();
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

void Player::InitSprite()
{
	std::string TextureFilePath = "./Assets/Textures/";

	std::vector<std::shared_ptr<kepler::ITexture2D>> textures;
	// load and walk & idle textures
	for (int i = 0; i < 3; i++)
	{
		std::string moveTexturePath = TextureFilePath + "walk" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, moveTexturePath));
	}
	m_animation[PlayerStateWalk].AddSprites({ textures[0], textures[1], textures[2] });
	m_animation[PlayerStateWalk].SetFrameCount(18);
	m_animation[PlayerStateWalk].SetRepeat(true);
	// set idle texture
	m_animation[PlayerStateIdle].AddSprites({ textures[0] });
	m_animation[PlayerStateIdle].SetFrameCount(4);
	m_animation[PlayerStateIdle].SetRepeat(false);

	textures.clear();
	// load and set jump texture
	for (int i = 0; i < 2; i++)
	{
		std::string jumpTexturePath = TextureFilePath + "jump" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, jumpTexturePath));
	}
	m_animation[PlayerStateJump].AddSprites({ textures[1], textures[0] });
	m_animation[PlayerStateJump].SetFrameCount(12);
	m_animation[PlayerStateJump].SetRepeat(true);
	textures.clear();

	// load and sset slide texture
	for (int i = 0; i < 3; i++)
	{
		std::string sildeTexturePath = TextureFilePath + "slide" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, sildeTexturePath));
	}
	m_animation[PlayerStateSlide].AddSprites({ textures[0] });
	m_animation[PlayerStateSlide].SetFrameCount(4);
	m_animation[PlayerStateSlide].SetRepeat(false);

	// load and set lose texture
	m_animation[PlayerStateLose].AddSprites({ textures[0], textures[1], textures[2] });
	m_animation[PlayerStateLose].SetFrameCount(18);
	m_animation[PlayerStateLose].SetRepeat(false);

	// load and set win texture
	auto winTexture = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, TextureFilePath + "win.png");
	m_animation[PlayerStateWin].AddSprites({ winTexture });
	m_animation[PlayerStateWin].SetFrameCount(1);
	m_animation[PlayerStateWin].SetRepeat(false);

	m_pCurAnim = &m_animation[PlayerStateIdle];
}

void Player::OnEvent(kepler::Event& e)
{

}

void Player::OnUpdate(float deltaTime)
{
#ifdef _DEBUG
	m_debugColor = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif
	int horizontal = 0;
	int vertical = 0;
	m_bIsSpiked = false;

	if (kepler::Input::IsButtonDown(kepler::key::Up))
	{
		vertical += 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::Down))
	{
		vertical -= 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::Left))
	{
		horizontal -= 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::Right))
	{
		horizontal += 1;
	}

	if (m_bIsGrounded)
	{
		if (vertical > 0)
		{
			m_bIsGrounded = false;
			m_state = ePlayerState::PlayerStateJump;
			m_size = constant::SQUIRTLE_JUMP_SIZE;
			if (m_pCurAnim != &m_animation[PlayerStateJump])
			{
				m_pCurAnim = &m_animation[PlayerStateJump];
				m_pCurAnim->Start();
			}
			m_curDirection = { horizontal * 5.0f, 25.0f };
		}
		else if (vertical < 0 && horizontal != 0)
		{
			if (kepler::Input::IsButtonDown(kepler::key::Space))
			{
				m_bIsGrounded = false;
				m_state = ePlayerState::PlayerStateSlide;
				m_size = constant::SQUIRTLE_SLIDE_SIZE;
				if (m_pCurAnim != &m_animation[PlayerStateSlide])
				{
					m_pCurAnim = &m_animation[PlayerStateSlide];
					m_pCurAnim->Start();
				}
				m_curDirection = { horizontal * 15.0f, 7.5f };
			}
		}
		else
		{
			m_size = constant::SQUIRTLE_IDLE_SIZE;
			if (horizontal)
			{
				m_state = ePlayerState::PlayerStateWalk;
				if (m_pCurAnim != &m_animation[PlayerStateWalk])
				{
					m_pCurAnim = &m_animation[PlayerStateWalk];
					m_pCurAnim->Start();
				}
				m_curDirection = { horizontal * 5.0f, 0.0f };
			}
			else
			{
				m_state = ePlayerState::PlayerStateIdle;
				if (m_pCurAnim != &m_animation[PlayerStateIdle])
				{
					m_pCurAnim = &m_animation[PlayerStateIdle];
					m_pCurAnim->Start();
				}
				m_curDirection = { 0.0f, 0.0f };
			}
		}
	}
	else
	{
		m_curDirection.y = m_lastDirection.y - (49.0f * deltaTime);
		switch (m_state)
		{
		case ePlayerState::PlayerStateSlide:
			{
				m_curDirection.x = m_lastDirection.x;
			}
			break;
		case ePlayerState::PlayerStateJump:
			{
				if (kepler::Input::IsButtonDown(kepler::key::Space))
				{
					m_bIsSpiked = true;
				}
				m_curDirection.x = horizontal * 5.0f;
			}
			break;
		}
	}

	m_position += m_curDirection;
	m_pCollider->SetSize(m_size);
	m_pCollider->SetPosition(m_position);
	m_lastDirection = m_curDirection;
	m_pCurAnim->Update();
}

void Player::OnRender()
{
	bool bFlipX = false;
	if (m_state != PlayerStateJump)
	{
		bFlipX = m_curDirection.x > 0.0f;
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
	kepler::Vec2f colliderPos = data.collider->GetPosition();
	kepler::Vec2f colliderSize = data.collider->GetSize();

	switch (data.collider->GetCategory())
	{
	case eColliderCategory::Net:
		{
			if (m_curDirection.x < 0.0f)
			{
				m_position.x = (m_size.x + colliderSize.x) / 2.0f;
			}
		}
		break;
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
	case eColliderCategory::Wall:
		{
			if (m_curDirection.x > 0.0f)
			{
				m_position.x = colliderPos.x - (colliderSize.x + m_size.x) / 2.0f;
			}
		}
		break;
	}
	m_pCollider->SetPosition(m_position);
	m_pCollider->SetSize(m_size);
}