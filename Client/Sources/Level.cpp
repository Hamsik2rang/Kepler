#include "Level.h"

#include "Constant.h"
#include "CollisionDetector.h"

void Level::Init(float width, float height)
{
	m_width = width;
	m_height = height;
	m_pLevelSprite = kepler::ITexture2D::Create(kepler::eTextureDataType::Float, "./Assets/Textures/court.png");

	m_pLeftWall = std::make_shared<Wall>(kepler::Vec2f{ -width / 2.0f - 5.0f, 0.0f }, kepler::Vec2f{ 10.0f, 5000.0f });
	m_pRightWall = std::make_shared<Wall>(kepler::Vec2f{ width / 2.0f, 0.0f }, kepler::Vec2f{ 10.0f, 5000.0f });
	m_pNet = std::make_shared<Net>(constant::NET_POSITION, kepler::Vec2f{constant::NET_SIZE.x, 2000.0f});
	m_pGround = std::make_shared<Ground>(constant::GROUND_POSITION, constant::GROUND_SIZE);

	CollisionDetector::AddCollider(m_pLeftWall);
	CollisionDetector::AddCollider(m_pRightWall);
	CollisionDetector::AddCollider(m_pNet);
	CollisionDetector::AddCollider(m_pGround);
}

void Level::OnRender()
{
	kepler::Renderer2D::Get()->DrawQuad({ 0.0f, 0.0f }, { m_width, m_height }, m_pLevelSprite);
#ifdef _DEBUG
	m_pLeftWall->OnRender();
	m_pRightWall->OnRender();
	m_pNet->OnRender();
	m_pGround->OnRender();

#endif
}

void Level::OnUpdate(float deltaTime)
{
	m_pLeftWall->OnUpdate(deltaTime);
	m_pRightWall->OnUpdate(deltaTime);
	m_pNet->OnUpdate(deltaTime);
	m_pGround->OnUpdate(deltaTime);
}
