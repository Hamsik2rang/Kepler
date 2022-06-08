#include "Level.h"

#include "Constant.h"
#include "CollisionDetector.h"

void Level::Init(float width, float height)
{
	m_width = width;
	m_height = height;
	m_pLevelSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/court.png");

	m_pLeftWall = std::make_shared<Wall>(kepler::Vec2f{ -width / 2.0f, 0.0f }, kepler::Vec2f{ 0.1f, 5000.0f });
	m_pRightWall = std::make_shared<Wall>(kepler::Vec2f{ width / 2.0f, 0.0f }, kepler::Vec2f{ 0.1f, 5000.0f });
	m_pNet = std::make_shared<Net>(constant::NET_POSITION, constant::NET_SIZE);
	m_pGround = std::make_shared<Ground>(constant::GROUND_POSITION, kepler::Vec2f{ 1500.0f, 0.1f });
	
	CollisionDetector::AddCollider(m_pLeftWall);
	CollisionDetector::AddCollider(m_pRightWall);
	CollisionDetector::AddCollider(m_pNet);
	CollisionDetector::AddCollider(m_pGround);
}

void Level::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad({ 0.0f, 0.0f }, { m_width, m_height }, m_pLevelSprite);
}

void Level::OnUpdate(float deltaTime)
{

}
