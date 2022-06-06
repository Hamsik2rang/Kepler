#include "Player.h"


static const int WIN_TEXTURE_INDEX = 0;
static const int LOSE_TEXTURE_INDEX = 1;


Player::Player(kepler::Vec2f& position, kepler::Vec2f& size, bool bIsGrounded, eColliderType colliderType)
	: GameObject(colliderType)
	, m_position{ position }
	, m_size{ size }
	, m_bIsGrounded{ bIsGrounded }
	, m_animationCallback{ nullptr }
{

}

void Player::Init()
{
	std::string TextureFilePath = "Assets/Textures/";
	for (int i = 0; i < 3; i++)
	{
		std::string moveTexturePath = TextureFilePath + "walk" + std::to_string(i + 1) + ".png";
		m_pMoveSprite[i] = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, moveTexturePath);
	}
	for (int i = 0; i < 2; i++)
	{
		std::string jumpTexturePath = TextureFilePath + "jump" + std::to_string(i + 1) + ".png";
		m_pJumpSprite[i] = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, jumpTexturePath);
		std::string sildeTexturePath = TextureFilePath + "slide" + std::to_string(i + 1) + ".png";
		m_pSlideSprite[i] = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, sildeTexturePath);
	}
	m_pEndGameSprite[0] = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, TextureFilePath + "win.png");
	m_pEndGameSprite[1] = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, TextureFilePath + "lose.png");
}

void Player::OnEvent(kepler::Event& e)
{
	
}

// space = power shot
// left/right = move
// up = jump
// down + spade = slide

void Player::OnUpdate(float deltaTime)
{
	if (m_bIsGrounded && m_position.y - (m_size.y / 2.0f) < -360.0f)
	{
		m_position.y = -360.0f + (m_size.y / 2.0f);
		m_bIsGrounded = true;
		// set idle animation

		return;
	}

	int horizontal = 0;
	int vertical = 0;

	if (kepler::Input::IsButtonDown(kepler::key::Space))
	{
		m_bIsSpiked = true;
	}

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

	m_direction.x = horizontal;

	if (m_bIsGrounded)
	{
		if (vertical == 0)
		{

		}

		if (m_bIsSpiked && vertical < 0)
		{
			// slide
			horizontal *= 100;
		}
		else if (vertical > 0)
		{
			// jump
		}
	}


	m_position += m_direction;
	// update anim

}

void Player::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad(m_position, m_size, m_nextRenderSprite, { 1.0f, 1.0f, 1.0f, 1.0f });
}
