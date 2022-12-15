#define _CRT_SECURE_NO_WARNINGS
#include "ResearchLayer.h"

#define WIDTH 1600	
#define HEIGHT 900

void ResearchLayer::OnAttach()
{
	m_pEntity = m_scene.CreateEntity();
	auto pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGBA8, WIDTH, HEIGHT);
	m_pEntity->AddComponent<kepler::SpriteRendererComponent>(pTexture, kepler::Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });
	m_camera.SetProjection(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 1.0f);
	m_camera.SetPosition(kepler::Vec3f::Zero);
}

void ResearchLayer::OnDetach()
{

}

void ResearchLayer::OnUpdate(float deltaTime)
{
	auto pSpriteRendererComponent = m_pEntity->GetComponent<kepler::SpriteRendererComponent>();
	uint8_t* pBuffer = new uint8_t[WIDTH * HEIGHT * 4];
	for (int row = 0; row < HEIGHT; row++)
	{
		for (int col = 0; col < WIDTH * 4; col += 4)
		{
			auto curByteIndex = row * WIDTH * 4 + col;

			const float xScale = 2.0f / (WIDTH * 4);
			const float yScale = 2.0f / HEIGHT;
			const float aspect = (float)WIDTH / HEIGHT;

			const float curCol = (col * xScale * aspect) - aspect;
			const float curRow = (row * yScale) - 1.0f;

			kepler::Vec3f origin{ curCol, curRow, 0.0f };
			const auto rayDir = kepler::Vec3f::Front;

			Ray pixelRay{ kepler::Vec3f{curCol, curRow, 0.0f}, rayDir };

			const kepler::Vec3f color = m_rayTracer.TraceRay(pixelRay);
			pBuffer[curByteIndex + 0] = (uint8_t)(std::min(color.r,1.0f) * 255.0f);
			pBuffer[curByteIndex + 1] = (uint8_t)(std::min(color.g,1.0f) * 255.0f);
			pBuffer[curByteIndex + 2] = (uint8_t)(std::min(color.b,1.0f) * 255.0f);
			pBuffer[curByteIndex + 3] = 255;
		}
	}
	pSpriteRendererComponent->pTexture->SetData(pBuffer, WIDTH, HEIGHT, 4);
	delete[] pBuffer;
}

void ResearchLayer::OnEvent(kepler::Event& e)
{

}

void ResearchLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);
	auto pTexture = m_pEntity->GetComponent<kepler::SpriteRendererComponent>()->pTexture;
	auto color = m_pEntity->GetComponent<kepler::SpriteRendererComponent>()->color;

	kepler::Renderer2D::Get()->DrawNonBatchedQuad({ 0.0f, 0.0f }, 0.0f, { WIDTH, HEIGHT }, pTexture, false, false, color);

	kepler::Renderer2D::Get()->EndScene();
}

void ResearchLayer::OnGUIRender()
{
	static float lightPos[3]{ 0.0f, 0.0f, -1.0f };
	static float diffuse[3]{ 0.0f, 0.0f, 1.0f };
	ImGui::Begin("Hi.");
	ImGui::Text("This is circle draw test.");
	ImGui::SliderFloat3("Center", (float*)&m_circleCenterPos, -2.0f, 2.0f);
	ImGui::ColorEdit3("Circle color", (float*)&m_circleColor);

	ImGui::SliderFloat("radius", &m_circleRadius, 0.0f, 1.0f);
	ImGui::SliderFloat3("Light", lightPos, -2.0f, 2.0f);
	ImGui::SliderFloat3("Diffuse", diffuse, 0.0f, 1.0f);

	m_rayTracer.GetSphere()->SetCenter(m_circleCenterPos);
	m_rayTracer.GetSphere()->SetRadius(m_circleRadius);
	m_rayTracer.GetSphere()->SetColor(m_circleColor);

	m_rayTracer.SetLightPos({ lightPos[0], lightPos[1], lightPos[2] });
	m_rayTracer.GetSphere()->SetDiffuse({ diffuse[0], diffuse[1], diffuse[2] });
	ImGui::End();
}