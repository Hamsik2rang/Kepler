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

			const kepler::Vec4f color = m_rayTracer.TraceRay(pixelRay);
			pBuffer[curByteIndex + 0] = (uint8_t)(color.r * 255.0f);
			pBuffer[curByteIndex + 1] = (uint8_t)(color.g * 255.0f);
			pBuffer[curByteIndex + 2] = (uint8_t)(color.b * 255.0f);
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
	static float color[3] = { m_circleColor.r / 255.0f, m_circleColor.g / 255.0f, m_circleColor.b / 255.0f };

	ImGui::Begin("Hi.");
	ImGui::Text("This is circle draw test.");
	ImGui::ColorEdit3("circle color", (float*)&color);
	m_circleColor.r = (uint8_t)(color[0] * 255.0f);
	m_circleColor.g = (uint8_t)(color[1] * 255.0f);
	m_circleColor.b = (uint8_t)(color[2] * 255.0f);

	ImGui::SliderFloat("x of center", &m_circleCenterPos.x, 0.0f, WIDTH);
	ImGui::SliderFloat("y of center", &m_circleCenterPos.y, 0.0f, HEIGHT);

	ImGui::SliderFloat("radius", &m_circleRadius, 0.0f, 360.0f);

	ImGui::End();
}