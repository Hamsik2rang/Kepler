#include "kepch.h"

// 외부 오픈소스 라이브러리 Dear ImGui를 이용합니다. ImGuiLayer의 추가 구현을 위한 자세한 설명과 사용법은 아래 링크를 참조하세요.
// https://github.com/ocornut/ImGui
#include "Core/GUI/ImGuiLayer.h"


namespace kepler {

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	
	}

	ImGuiLayer::~ImGuiLayer()
	{

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
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		// TODO: 이벤트를 카테고리에 맞게 처리(Dispatcher이용?)
	}

}