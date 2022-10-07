#define _CRT_SECURE_NO_WARNINGS
#include "ResearchLayer.h"


void ResearchLayer::OnAttach()
{
	m_pImage.ReadFromFile("./Assets/Textures/image_1.JPG");

	auto pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::Float_RGBA32, m_pImage.width, m_pImage.height, m_pImage.height);
	//pTexture->SetData(m_pImage.rowData, m_pImage.width, m_pImage.height);
	pEntity = m_scene.CreateEntity();
	pEntity->AddComponent<kepler::SpriteRendererComponent>(pTexture, kepler::Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });
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
	kepler::Renderer2D::Get()->DrawQuad(kepler::Mat44f::Identity,pTexture, false, false, color);

	kepler::Renderer2D::Get()->EndScene();
}

void ResearchLayer::OnGUIRender()
{

}
