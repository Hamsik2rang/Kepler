#include "kepch.h"

// 외부 오픈소스 라이브러리 Dear ImGui를 이용합니다. ImGuiLayer의 추가 구현을 위한 자세한 설명과 사용법은 아래 링크를 참조하세요.
// https://github.com/ocornut/ImGui
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/GraphicsContext.h"


	// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ImFont* g_defaultFont;


namespace kepler {

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnDetach();
	}

	void ImGuiLayer::OnAttach()
	{
		// ImGui context 생성
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;


		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application* pApp = Application::Get();
		HWND hWnd = pApp->GetWindow()->GetWindowHandle();

		ID3D11Device* pDevice = IGraphicsContext::Get()->GetDevice();
		ID3D11DeviceContext* pDeviceContext = IGraphicsContext::Get()->GetDeviceContext();
		// ImGui 초기화
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		g_defaultFont = io.Fonts->AddFontFromFileTTF("../Kepler/Resources/OpenSans-Regular.ttf", 18.0f);
	}

	void ImGuiLayer::OnDetach()
	{
		// TODO: 해제할 리소스들 있으면 해제
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{

	}

	void ImGuiLayer::OnEvent(Event& e)
	{

	}

	void ImGuiLayer::Begin()
	{
		//현재 프레임에 대한 ImGui 컨텍스트 초기화
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}

	void ImGuiLayer::End()
	{
		// 현재 프레임 ImGui 컨텍스트 안에 그려진 모든 레이어들 그리기
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

	}

	void ImGuiLayer::OnGUIRender()
	{
		// GUI 예제 확인용
		// GUI 붙이다 헷갈리는거 있을때 주석 풀고 데모 GUI 확인하세요.
		//ImGui::PushFont(g_defaultFont);
		//ImGuiIO& io = ImGui::GetIO();

		//static bool bIsShownDemo = false;
		//if (bIsShownDemo)
		//{
		//	ImGui::ShowDemoWindow(&bIsShownDemo);
		//}

		//static float f = 0.0f;

		//ImGui::Begin("Hello Kepler!");
		//ImGui::Text("This is Test..");
		//ImGui::Checkbox("demo window", &bIsShownDemo);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		//static ImVec4 clearColor(0.45f, 0.55f, 0.60f, 1.0f);
		//ImGui::ColorEdit3("clear color", (float*)&clearColor);

		//static int counter = 0;
		//if (ImGui::Button("Button"))
		//{
		//	counter++;
		//}
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//ImGui::End();
		//ImGui::PopFont();
	}

	LRESULT ImGuiLayer::ImGuiEventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}