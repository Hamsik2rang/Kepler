#include "Player.h"


Player::Player(const kepler::Vec2f& position, const kepler::Vec2f& size, bool bIsGrounded, eColliderType colliderType)
	: GameObject(colliderType)
	, m_position{ position }
	, m_lastDirection{ -1.0f, 0.0f }
	, m_size{ size }
	, m_bIsGrounded{ bIsGrounded }
	, m_state{ ePlayerState::PlayerStateIdle }
{
	Init();
}

void Player::Init()
{
	std::string TextureFilePath = "./Assets/Textures/";
	
	std::vector<std::shared_ptr<kepler::ITexture2D>> textures;
	// load and set slide texture
	for (int i = 0; i < 3; i++)
	{
		std::string moveTexturePath = TextureFilePath + "walk" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, moveTexturePath));
	}
	m_animation[PlayerStateWalk].AddSprite(textures[0]);
	m_animation[PlayerStateWalk].AddSprites({textures[0], textures[1], textures[2]});
	m_animation[PlayerStateWalk].SetFrameCount(6);
	m_animation[PlayerStateWalk].SetRepeat(true);
	// set idle texture
	m_animation[PlayerStateIdle].AddSprites({ textures[0] });
	m_animation[PlayerStateIdle].SetFrameCount(1);
	m_animation[PlayerStateIdle].SetRepeat(false);
	
	textures.clear();
	// load and set jump texture
	for (int i = 0; i < 2; i++)
	{
		std::string jumpTexturePath = TextureFilePath + "jump" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, jumpTexturePath));
	}
	m_animation[PlayerStateJump].AddSprites({ textures[0], textures[1] });
	m_animation[PlayerStateJump].SetFrameCount(4);
	m_animation[PlayerStateJump].SetRepeat(false);
	textures.clear();

	// load and sset slide texture
	for (int i = 0; i < 3; i++)
	{
		std::string sildeTexturePath = TextureFilePath + "slide" + std::to_string(i + 1) + ".png";
		textures.push_back(kepler::ITexture2D::Create(kepler::eTextureDataType::Float, sildeTexturePath));
	}
	m_animation[PlayerStateSlide].AddSprites({ textures[0] });
	m_animation[PlayerStateSlide].SetFrameCount(1);
	m_animation[PlayerStateSlide].SetRepeat(false);

	// load and set lose texture
	m_animation[PlayerStateLose].AddSprites({ textures[0], textures[1], textures[2] });
	m_animation[PlayerStateLose].SetFrameCount(6);
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
	if (m_bIsGrounded && m_position.y - (m_size.y / 2.0f) < -360.0f)
	{
		m_position.y = -360.0f + (m_size.y / 2.0f);
		m_bIsGrounded = true;
		m_curDirection = { 0.0f, 0.0f };
		m_state = ePlayerState::PlayerStateIdle;
		m_pCurAnim = &m_animation[PlayerStateIdle];
		m_pCurAnim->Start();
	}

	int horizontal = 0;
	int vertical = 0;

	if (kepler::Input::IsButtonDown(kepler::key::W))
	{
		vertical += 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::S))
	{
		vertical -= 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::A))
	{
		horizontal -= 1;
	}
	if (kepler::Input::IsButtonDown(kepler::key::D))
	{
		horizontal += 1;
	}

	if (m_bIsGrounded)
	{
		if (vertical > 0)
		{
			m_state = ePlayerState::PlayerStateJump;
			m_pCurAnim = &m_animation[PlayerStateJump];
			m_pCurAnim->Start();
			m_curDirection = { horizontal * 5.0f, 20.0f };
		}
		else if (vertical < 0 && horizontal != 0)
		{
			m_state = ePlayerState::PlayerStateSlide;
			m_pCurAnim = &m_animation[PlayerStateSlide];
			m_pCurAnim->Start();
			m_curDirection = { horizontal * 20.0f, 5.0f };
		}
		else
		{
			if (horizontal)
			{
				m_state = ePlayerState::PlayerStateWalk;
				m_pCurAnim = &m_animation[PlayerStateWalk];
				m_pCurAnim->Start();
				m_curDirection = { horizontal * 5.0f, 0.0f };
			}
			else
			{
				m_state = ePlayerState::PlayerStateIdle;
				m_pCurAnim = &m_animation[PlayerStateIdle];
				m_pCurAnim->Start();
				m_curDirection = { 0.0f, 0.0f };
			}
		}
	}
	else
	{
		m_curDirection.y = m_lastDirection.y - 9.8f * deltaTime;
		switch (m_state)
		{
		case ePlayerState::PlayerStateSlide:
			{
				m_curDirection.x = m_lastDirection.x;
			}
			break;
		case ePlayerState::PlayerStateJump:
			{
				m_curDirection.x = horizontal * 5.0f;
			}
			break;
		}
	}

	m_position += m_curDirection;
	m_curDirection = m_lastDirection;
	m_pCurAnim->Update();
}

void Player::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_pCurAnim->GetCurFrameSprite(), {1.0f, 1.0f, 1.0f, 1.0f});
}
