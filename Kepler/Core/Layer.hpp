#pragma once

#include <string>

#include "Core/Base.h"
#include "Core/Event/Event.hpp"

namespace kepler {

	class Layer
	{
	protected:
		std::string m_name;

	public:
		Layer(const std::string& name = "Layer")
			: m_name(name)
		{}
		virtual ~Layer() = default;

		// ���̾ �����쿡 �߰��մϴ�
		virtual void OnAttach() {};
		// ���̾ �����쿡�� �����մϴ�
		virtual void OnDetach() {};
		// ������Ʈ �������� ȣ��˴ϴ�.
		virtual void OnUpdate(float deltaTime) {};
		// ���̾�� ���޵� �̺�Ʈ�� ó���մϴ�(�Ǵ� �����մϴ�).
		virtual void OnEvent(Event& e) {};
		// ���̾�� ȭ�鿡 �׸� �͵��� �׸��ϴ�.
		virtual void OnRender() {};

		// ImGUI ������ ���� �Լ����Դϴ�.
		virtual void Begin() {};
		virtual void End() {};
		virtual void OnGUIRender() {};

		inline const std::string& GetName() const { return m_name; }
	};
}