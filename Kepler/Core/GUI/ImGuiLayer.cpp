#include "kepch.h"

// �ܺ� ���¼ҽ� ���̺귯�� Dear ImGui�� �̿��մϴ�. ImGuiLayer�� �߰� ������ ���� �ڼ��� ����� ������ �Ʒ� ��ũ�� �����ϼ���.
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
		// TODO: ������ ���ҽ��� ������ ����
	}

	void ImGuiLayer::OnUpdate()
	{
		// TODO: ������Ʈ �������� �� �� ����
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		// TODO: �̺�Ʈ�� ī�װ��� �°� ó��(Dispatcher�̿�?)
	}

}