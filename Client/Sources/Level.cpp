#include "Level.h"

void Level::Init(float width, float height)
{
	m_width = width;
	m_height = height;
	m_pLevelSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/court.png");
}

void Level::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad({ 0.0f, 0.0f }, { m_width, m_height }, m_pLevelSprite);
}

void Level::OnUpdate(float deltaTime)
{

}
