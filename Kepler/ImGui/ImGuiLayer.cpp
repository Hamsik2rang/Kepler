#include "kepch.h"

// 외부 오픈소스 라이브러리 Dear ImGui를 이용합니다. ImGuiLayer의 추가 구현을 위한 자세한 설명과 사용법은 아래 링크를 참조하세요.
// https://github.com/ocornut/ImGui
#include "imgui.h"
#include "ImGui/ImGuiSource/ImGuiDX11Renderer.h"
#include "ImGui/ImGuiSource/ImGuiWin32Renderer.h"

#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"

namespace kepler {

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// TODO: 이거 언젠가 꼭 바꿔야됩니다..
		HWND hWnd = ((WindowsWindow*)&Application::Get()->GetWindow())->GetWindowHandle();
		ImGui_ImplDX11_Init(
			((WindowsWindow*)&Application::Get()->GetWindow())->GetD3DDevice(), 
			((WindowsWindow*)&Application::Get()->GetWindow())->GetD3DDeviceContext());
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		// TODO: Create ImGui Context and set props

	}

	void ImGuiLayer::OnDetach()
	{
		// TODO: 해제할 리소스들 있으면 해제
	}

	void ImGuiLayer::OnUpdate()
	{
		// TODO: 업데이트 루프마다 할 일 지정
		ImGuiIO& io = ImGui::GetIO();
		Application* pApp = Application::Get();
		io.DisplaySize = ImVec2(pApp->GetWindow().GetWidth(), pApp->GetWindow().GetHeight());
		ImGui_ImplDX11_NewFrame();
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

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		// TODO: 이벤트를 카테고리에 맞게 처리(Dispatcher이용?)
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&OnKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&OnKeyReleasedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&OnMouseMovedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OnMouseScrolledEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&OnMouseButtonReleasedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&OnMouseButtonPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OnWindowResizeEvent, this, std::placeholders::_1));
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGui::GetIO().MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		return false;
	}


}