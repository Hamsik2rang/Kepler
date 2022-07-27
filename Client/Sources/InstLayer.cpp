#include "InstLayer.h"
#include "Core/Application.h"
#include "Renderer/RenderState.h"

void InstLayer::OnAttach()
{
	auto& window = kepler::Application::Get()->GetWindow();
	m_screenHeight = static_cast<float>(window.GetHeight());
	m_screenWidth = static_cast<float>(window.GetWidth());

	auto ds = kepler::IRenderState::Get()->GetDepthState();
	ds.bDepthTest = false;
	ds.bDepthWrite = false;
	kepler::IRenderState::Get()->SetDepthState(ds);

	m_camera.SetProjection(0, m_screenWidth, 0, m_screenHeight, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });

	auto blendDesc = kepler::IRenderState::Get()->GetBlendState();
	blendDesc.renderTarget[0].bBlendEnable = true;
	blendDesc.renderTarget[0].writeMask = 0b00001111;
	blendDesc.renderTarget[0].srcColorFactor = kepler::eBlendFactor::SrcAlpha;
	blendDesc.renderTarget[0].destColorFactor = kepler::eBlendFactor::InvSrcAlpha;
	blendDesc.renderTarget[0].colorBlendOperator = kepler::eBlendOperator::Add;
	blendDesc.renderTarget[0].srcAlphaFactor = kepler::eBlendFactor::InvDestAlpha;
	blendDesc.renderTarget[0].destAlphaFactor = kepler::eBlendFactor::One;
	blendDesc.renderTarget[0].alphaBlendOperator = kepler::eBlendOperator::Add;
	blendDesc.customFactor[0] = 0.0f;
	blendDesc.customFactor[1] = 0.0f;
	blendDesc.customFactor[2] = 0.0f;
	blendDesc.customFactor[3] = 0.0f;
	blendDesc.sampleMask = 0xffffffff;
	kepler::IRenderState::Get()->SetBlendState(blendDesc);
	kepler::IRenderState::Get()->Bind();

	ImGuiIO io = ImGui::GetIO();
	m_font = io.Fonts->AddFontFromFileTTF("./Assets/OpenSans-Regular.ttf", 72.0f);
}

void InstLayer::OnDetach()
{

}

void InstLayer::OnUpdate(const float deltaTime)
{
	m_particleEngine.OnUpdate(deltaTime);
}

void InstLayer::OnRender()
{
	kepler::IRenderState::Get()->Bind();
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	m_particleEngine.OnRender(m_bIsBatchRendering);

	kepler::Renderer2D::Get()->EndScene();
}

void InstLayer::OnGUIRender()
{
	if (m_bIsBatchRendering)
	{
		ImGui::GetForegroundDrawList()->AddText(m_font, 72.0f, ImGui::GetWindowPos(), 0xffffffff, "Batch ON");
	}
	else
	{
		ImGui::GetForegroundDrawList()->AddText(m_font, 72.0f, ImGui::GetWindowPos(), 0xffffffff, "Batch OFF");
	}
}

void InstLayer::OnEvent(kepler::Event& e)
{
	kepler::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<kepler::MouseMovedEvent>(std::bind(&InstLayer::OnMouseMovedEvent, this, std::placeholders::_1));
	dispatcher.Dispatch<kepler::KeyPressedEvent>(std::bind(&InstLayer::OnKeyPressedEvent, this, std::placeholders::_1));
}


bool InstLayer::OnKeyPressedEvent(kepler::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == kepler::key::Space)
	{
		m_bIsBatchRendering ^= true;
	}

	return true;
}

bool InstLayer::OnMouseMovedEvent(kepler::MouseMovedEvent& e)
{
	static bool bIsInitialMove = true;

	float curCursorX = e.GetX() - (m_screenWidth / 2.0f);
	float curCursorY = (m_screenHeight / 2.0f) - e.GetY();
	if (bIsInitialMove)
	{
		bIsInitialMove = false;
		m_lastCursorX = curCursorX;
		m_lastCursorY = curCursorY;
		return false;
	}

	kepler::Vec2f position = { curCursorX, curCursorY };
	kepler::Vec2f velocity = { m_lastCursorX - curCursorX, m_lastCursorY - curCursorY };
	velocity = velocity.Normalize() * 1000.0f;
	float sizeBegin = 2.0f;
	float sizeEnd = 20.0f;
	float rotation = kepler::Random::Float(0.0f, 90.0f);
	for (int i = 0; i < 10; i++)
	{
		auto curVelocity = velocity;
		curVelocity.x *= kepler::Random::Float(0.2f, 1.8f);
		curVelocity.y *= kepler::Random::Float(0.2f, 1.8f);
		auto velocityAdd = velocity * kepler::Random::Float(-0.02f, 0.02f);
		kepler::Vec4f colorBegin = { kepler::Random::Float(0.0f, 1.0f), kepler::Random::Float(0.0f, 1.0f), kepler::Random::Float(0.0f, 1.0f), 1.0f };
		kepler::Vec4f colorEnd = { colorBegin.x, colorBegin.y, colorBegin.z, 0.0f };

		m_particleEngine.Emit(position, curVelocity, velocityAdd, colorBegin, colorEnd, rotation, sizeBegin, sizeEnd);
	}

	m_lastCursorX = curCursorX;
	m_lastCursorY = curCursorY;
}