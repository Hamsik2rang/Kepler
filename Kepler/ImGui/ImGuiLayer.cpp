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
		// TODO: Create ImGui Context and set props
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
	}

	void ImGuiLayer::OnDetach()
	{
		// TODO: 해제할 리소스들 있으면 해제
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// ImGui 새 프레임 생성
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool bIsShownDemo = true;
		if (bIsShownDemo)
		{
			ImGui::ShowDemoWindow(&bIsShownDemo);
		}
		
		static float f = 0.0f;

		ImGui::Begin("Hello Kepler!");
		ImGui::Text("This is some useful text");
		ImGui::Checkbox("demo window", &bIsShownDemo);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		static ImVec4 clearColor(0.45f, 0.55f, 0.60f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clearColor);

		static int counter = 0;
		if (ImGui::Button("Button"))
		{
			counter++;
		}
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{

	}

	LRESULT ImGuiLayer::ImGuiEventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}