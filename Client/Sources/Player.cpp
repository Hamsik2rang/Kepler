#include "Player.h"


static const int WIN_TEXTURE_INDEX = 0;
static const int LOSE_TEXTURE_INDEX = 1;

void Player::Init(bool bIsEnemy)
{
	// Set Enemy Position
	if (bIsEnemy)
	{

	}
	// Set Player Position
	else
	{
		m_position = { 150.0f, -300.0f };

	}
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
