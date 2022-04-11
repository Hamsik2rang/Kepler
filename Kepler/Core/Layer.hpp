#pragma once

#include <string>

#include "Core/Base.h"
#include "Core/Event/Event.hpp"

namespace kepler {

	class KEPLER_API Layer
	{
	protected:
		std::string m_name;

	public:
		Layer(const std::string& name = "Layer")
			: m_name(name)
		{}
		virtual ~Layer() = default;

		// ���̾ �����쿡 �߰��մϴ�
		virtual void OnAttach() = 0;
		// ���̾ �����쿡�� �����մϴ�
		virtual void OnDetach() = 0;
		// ������Ʈ �������� ȣ��˴ϴ�.
		virtual void OnUpdate() = 0;
		// ���̾�� ���޵� �̺�Ʈ�� ó���մϴ�(�Ǵ� �����մϴ�).
		virtual void OnEvent(Event& e) = 0;

		inline const std::string& GetName() const { return m_name; }
	};
}