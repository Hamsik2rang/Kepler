#include "kepch.h"

// �ܺ� ���¼ҽ� ���̺귯�� Dear ImGui�� �̿��մϴ�. ImGuiLayer�� �߰� ������ ���� �ڼ��� ����� ������ �Ʒ� ��ũ�� �����ϼ���.
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
		// ImGui context ����
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
		// ImGui �ʱ�ȭ
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		g_defaultFont = io.Fonts->AddFontFromFileTTF("../Kepler/Resources/OpenSans-Regular.ttf", 18.0f);
	}

	void ImGuiLayer::OnDetach()
	{
		// TODO: ������ ���ҽ��� ������ ����
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
		//���� �����ӿ� ���� ImGui ���ؽ�Ʈ �ʱ�ȭ
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}

	void ImGuiLayer::End()
	{
		// ���� ������ ImGui ���ؽ�Ʈ �ȿ� �׷��� ��� ���̾�� �׸���
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
		// GUI ���� Ȯ�ο�
		// GUI ���̴� �򰥸��°� ������ �ּ� Ǯ�� ���� GUI Ȯ���ϼ���.
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