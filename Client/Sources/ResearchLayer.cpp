#define _CRT_SECURE_NO_WARNINGS
#include "ResearchLayer.h"


void ResearchLayer::OnAttach()
{
	m_pImage.ReadFromFile("./Assets/Textures/image_1.jpg");

	auto pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGB8,"./Assets/Textures/image_1.jpg");
	//auto pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGBA8, m_pImage.width, m_pImage.height);
	//pTexture->SetData(m_pImage.rawData, m_pImage.width, m_pImage.height, m_pImage.channel);
	pEntity = m_scene.CreateEntity();
	pEntity->AddComponent<kepler::SpriteRendererComponent>(pTexture, kepler::Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });

	m_camera.SetPosition(kepler::Vec3f::Zero);
	m_camera.SetRotation(0.0f);
	m_camera.SetProjection(0.0f, 1280.0f, 0.0f, 720.0f, 0.0f, 1.0f);
}

void ResearchLayer::OnDetach()
{

}

void ResearchLayer::OnUpdate(float deltaTime)
{

}

void ResearchLayer::OnEvent(kepler::Event& e)
{

}

void ResearchLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);
	auto pTexture = pEntity->GetComponent<kepler::SpriteRendererComponent>()->pTexture;
	auto color = pEntity->GetComponent<kepler::SpriteRendererComponent>()->color;

	kepler::Renderer2D::Get()->DrawNonBatchedQuad({ 0.0f, 0.0f }, 0.0f, { 1280.0f, 720.0f }, pTexture, false, false, color);

	kepler::Renderer2D::Get()->EndScene();
}

void ResearchLayer::OnGUIRender()
{

}
