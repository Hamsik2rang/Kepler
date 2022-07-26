#include "InstLayer.h"
#include "Core/Application.h"
#include "Renderer/RenderState.h"

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
	kepler::Vec4f colorBegin = { 0.0f, 0.8f, 1.0f, 1.0f };
	kepler::Vec4f colorEnd = { 0.0f, 0.8f, 1.0f, 0.0f };
	float sizeBegin = 2.0f;
	float sizeEnd = 20.0f;
	float rotation = kepler::math::DegToRad(kepler::Random::Float(0.0f, 45.0f));
	for (int i = 0; i < 5; i++)
	{
		auto curVelocity = velocity;
		curVelocity.x *= kepler::Random::Float(0.2f, 1.8f);
		curVelocity.y *= kepler::Random::Float(0.2f, 1.8f);
		auto velocityAdd = velocity * kepler::Random::Float(-0.02f, 0.02f);

		m_particleEngine.Emit(position, curVelocity, velocityAdd, colorBegin, colorEnd, rotation, sizeBegin, sizeEnd);
	}

	m_lastCursorX = curCursorX;
	m_lastCursorY = curCursorY;
}

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

	//TODO: Alpha Blend State 추가하기
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
	kepler::Renderer2D::Get()->BeginScene(m_camera);
	kepler::IRenderState::Get()->Bind();

	m_particleEngine.OnRender();

	kepler::Renderer2D::Get()->EndScene();
}

void InstLayer::OnEvent(kepler::Event& e)
{
	kepler::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<kepler::MouseMovedEvent>(std::bind(&InstLayer::OnMouseMovedEvent, this, std::placeholders::_1));
}
