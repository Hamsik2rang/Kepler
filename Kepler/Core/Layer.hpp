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

		// 레이어를 윈도우에 추가합니다
		virtual void OnAttach() = 0;
		// 레이어를 윈도우에서 제거합니다
		virtual void OnDetach() = 0;
		// 업데이트 루프마다 호출됩니다.
		virtual void OnUpdate() = 0;
		// 레이어에게 전달된 이벤트를 처리합니다(또는 무시합니다).
		virtual void OnEvent(Event& e) = 0;

		inline const std::string& GetName() const { return m_name; }
	};
}