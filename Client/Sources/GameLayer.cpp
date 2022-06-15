#include "GameLayer.h"
#include "CollisionDetector.h"

#include <imgui.h>


GameLayer::GameLayer()
	: m_playerScore{ 0u }
	, m_enemyScore{ 0u }
	, m_pSolidFont{nullptr}
	, m_pDebugFont{ nullptr }
	, m_pHollowFont{ nullptr }
	, m_time{ 0.0f }
	, m_readyTime{ 0.0f }
	, m_bBlink{ true }
{

}

void GameLayer::OnAttach()
{
	kepler::IWindow* window = kepler::Application::Get()->GetWindow();
	float height = static_cast<float>(window->GetHeight());
	float width = static_cast<float>(window->GetWidth());

	m_camera.SetProjection(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });

	m_pPlayer = std::make_shared<Player>(constant::PLAYER_SPAWN_POSITION, constant::SQUIRTLE_IDLE_SIZE);
	m_pBall = std::make_shared<Ball>(constant::BALL_PLAYER_SPAWN_POSITION, constant::BALL_SIZE.x);
	m_pEnemy = std::make_shared<Enemy>(kepler::Vec2f{ -constant::PLAYER_SPAWN_POSITION.x, constant::PLAYER_SPAWN_POSITION.y }, constant::SQUIRTLE_IDLE_SIZE, m_pPlayer, m_pBall);
	m_pLevel = std::make_shared<Level>();
	m_pLevel->Init(width, height);

	// GUI용 폰트 초기화
	ImGuiIO& io = ImGui::GetIO();
	m_pSolidFont = io.Fonts->AddFontFromFileTTF("./Assets/Pokemon-Solid.ttf", 180.0f);
	m_pHollowFont = io.Fonts->AddFontFromFileTTF("Assets/Pokemon-Hollow.ttf", 180.0f);
	m_pDebugFont = io.Fonts->AddFontFromFileTTF("./Assets/OpenSans-Regular.ttf", 18.0f);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(float deltaTime)
{
	m_time += deltaTime;

	switch (m_state)
	{
	case eGameState::Menu:
		{
			if (kepler::Input::IsButtonDown(kepler::key::Space))
			{
				m_state = eGameState::Ready;
			}
		}
		break;
	case eGameState::Ready:
		{
			// respawn & draw once



			// count ready time
			m_readyTime += deltaTime;
			if (m_readyTime > 3.0f)
			{
				m_state = eGameState::Play;
				m_readyTime = 0.0f;
			}
		}
		break;
	case eGameState::Pause:
		{
			if (kepler::Input::IsButtonDown(kepler::key::Enter))
			{
				m_state = eGameState::Ready;
			}
		}
		break;
	case eGameState::Play:
		{
			m_pLevel->OnUpdate(deltaTime);
			m_pBall->OnUpdate(deltaTime);
			m_pPlayer->OnUpdate(deltaTime);
			m_pEnemy->OnUpdate(deltaTime);
			CollisionDetector::Detection();

			if (kepler::Input::IsButtonDown(kepler::key::P))
			{
				m_state = eGameState::Pause;
			}
			if (m_pBall->IsGrounded())
			{
				m_state = eGameState::GameOver;
				if (m_pBall->GetPosition().x < 0.0f)
				{
					m_playerScore++;
				}
				else
				{
					m_enemyScore++;
				}
			}
		}
		break;
	case eGameState::GameOver:
		{
			if (m_playerScore >= 15 || m_enemyScore >= 15)
			{
				m_bIsGameOver = true;
			}
			else
			{
				m_state = eGameState::Ready;
			}
		}
		break;
	}

}

// 렌더링 함수
void GameLayer::OnRender()
{
	// 현재 카메라의 viewprojection을 지정해 화면 렌더링 준비
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	// 나중에 그릴수록 위에 그려짐(rendering order)
	m_pLevel->OnRender();
	m_pPlayer->OnRender();
	m_pEnemy->OnRender();
	m_pBall->OnRender();

	// 렌더링 종료
	kepler::Renderer2D::Get()->EndScene();
}

// GUI 렌더링 함수
void GameLayer::OnGUIRender()
{
	ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;
	ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	ImGui::PushFont(m_pSolidFont);
	switch (m_state)
	{
	case eGameState::Menu:
		{
			ImVec2 textSize = ImGui::CalcTextSize("Squirtle");
			ImVec2 textPos = { viewportPos.x + (viewportSize.x - textSize.x) / 2.0f, viewportPos.y + 100.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, "Squirtle");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, "Squirtle");

			textSize = ImGui::CalcTextSize("VolleyBall");
			textPos = { viewportPos.x + (viewportSize.x - textSize.x) / 2.0f, textPos.y + 200.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, "VolleyBall");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, "VolleyBall");

			auto transparency = (int)((1.0f - (std::cosf(m_time * 5.0f) + 1.0f) / 2.0f) * 255.0f) << 24;
			textPos.x += 80.0f;
			textPos.y += 200.0f;
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 60.0f, textPos, 0x00aa5507 | transparency, "Press SPACE to Play!");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 60.0f, textPos, 0x0000ceff | transparency, "Press SPACE to Play!");
		}
		break;
	case eGameState::Ready:
		{
			ImVec2 textSize = ImGui::CalcTextSize("1");
			ImVec2 textPos = { viewportPos.x + (viewportSize.x - textSize.x) / 2.0f, viewportPos.y + 100.0f };
			if (m_readyTime < 1.0f)
			{
				auto transparency = (int)((1.0f - m_readyTime) * 255.0f) << 24;
				ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0x00aa5507 | transparency, "3");
				ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0x0000ceff | transparency, "3");

			}
			else if (m_readyTime > 1.0f && m_readyTime < 2.0f)
			{
				auto transparency = (int)((2.0f - m_readyTime) * 255.0f) << 24;
				ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0x00aa5507 | transparency, "2");
				ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0x0000ceff | transparency, "2");
			}
			else
			{
				auto transparency = (int)((3.0f - m_readyTime) * 255.0f) << 24;
				ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0x00aa5507 | transparency, "1");
				ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0x0000ceff | transparency, "1");
			}

		}
		break;
	case eGameState::Play:
		{
			std::string scoreStr = std::to_string(m_enemyScore);
			ImVec2 textSize = ImGui::CalcTextSize(scoreStr.c_str());
			ImVec2 textPos = { viewportPos.x + (viewportSize.x / 2.0f - textSize.x) / 2.0f, viewportPos.y + 10.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, scoreStr.c_str());
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, scoreStr.c_str());

			textSize = ImGui::CalcTextSize("-");
			textPos = { viewportPos.x + (viewportSize.x - textSize.x) / 2.0f, viewportPos.y + 10.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, "-");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, "-");

			scoreStr = std::to_string(m_playerScore);
			textSize = ImGui::CalcTextSize(scoreStr.c_str());
			textPos = { viewportPos.x + viewportSize.x / 2.0f + (viewportSize.x / 2.0f - textSize.x) / 2.0f, viewportPos.y + 10.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, scoreStr.c_str());
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, scoreStr.c_str());
		}
		break;
	case eGameState::Pause:
		{
			ImVec2 textSize = ImGui::CalcTextSize("Paused");
			ImVec2 textPos = { viewportPos.x + (viewportSize.x - textSize.x) / 2.0f, viewportPos.y + 200.0f };
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 180.0f, textPos, 0xffaa5507, "Paused");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 180.0f, textPos, 0xff00ceff, "Paused");

			textPos.x -= 30.0f;
			textPos.y += 200.0f;
			ImGui::GetForegroundDrawList()->AddText(m_pSolidFont, 60.0f, textPos, 0xffaa5507, "Press ENTER to Resume");
			ImGui::GetForegroundDrawList()->AddText(m_pHollowFont, 60.0f, textPos, 0xff00ceff, "Press ENTER to Resume");
		}
		break;
	case eGameState::GameOver:
		{

		}
		break;
	}
	ImGui::PopFont();

	// 디버그 전용 GUI
#ifdef _DEBUG
	ImGui::PushFont(m_pDebugFont);
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Debug Mode");
	ImGui::Text("Press R to Respaw Ball");
	ImGui::NewLine();
	// Player Info
	kepler::Vec2f playerPos = m_pPlayer->GetPosition();
	kepler::Vec2f playerSize = m_pPlayer->GetSize();
	kepler::Vec2f playerDir = m_pPlayer->GetCurrentDirection();
	kepler::Vec2f playerLastDir = m_pPlayer->GetLastDirection();
	bool bIsPlayerSpiked = *reinterpret_cast<bool*>(m_pBall->GetCollisionInfo());
	ImGui::Text("Player");
	ImGui::Text("Position (%.2f, %.2f)", playerPos.x, playerPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", playerSize.x, playerSize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", playerDir.x, playerDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", playerLastDir.x, playerLastDir.y);
	ImGui::Text("Spike: %d", bIsPlayerSpiked);

	ImGui::NewLine();

	kepler::Vec2f ballPos = m_pBall->GetPosition();
	kepler::Vec2f ballSize = m_pBall->GetSize();
	kepler::Vec2f ballDir = m_pBall->GetCurrentDirection();
	kepler::Vec2f ballLastDir = m_pBall->GetLastDirection();
	bool bIsBallAccelerated = *reinterpret_cast<bool*>(m_pBall->GetCollisionInfo());
	ImGui::Text("Ball");
	ImGui::Text("Position: (%.2f, %.2f)", ballPos.x, ballPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", ballSize.x, ballSize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", ballDir.x, ballDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", ballLastDir.x, ballLastDir.y);
	ImGui::Text("Accelerated: %d", bIsBallAccelerated);

	ImGui::NewLine();

	kepler::Vec2f enemyPos = m_pEnemy->GetPosition();
	kepler::Vec2f enemySize = m_pEnemy->GetSize();
	kepler::Vec2f enemyDir = m_pEnemy->GetCurrentDirection();
	kepler::Vec2f enemyLastDir = m_pEnemy->GetLastDirection();
	ImGui::Text("Enemy");
	ImGui::Text("Position (%.2f, %.2f)", enemyPos.x, enemyPos.y);
	ImGui::Text("Size: (%.2f, %.2f)", enemySize.x, enemySize.y);
	ImGui::Text("Direction: (%.2f, %.2f)", enemyDir.x, enemyDir.y);
	ImGui::Text("Last Direction : (%.2f, %.2f)", enemyLastDir.x, enemyLastDir.y);

	ImGui::End();
	ImGui::PopFont();
#endif
}

void GameLayer::OnEvent(kepler::Event& e)
{

}