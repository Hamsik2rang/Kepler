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

		// 레이어를 윈도우에 추가합니다
		virtual void OnAttach() {};
		// 레이어를 윈도우에서 제거합니다
		virtual void OnDetach() {};
		// 업데이트 루프마다 호출됩니다.
		virtual void OnUpdate(float deltaTime) {};
		// 레이어에게 전달된 이벤트를 처리합니다(또는 무시합니다).
		virtual void OnEvent(Event& e) {};
		// 레이어에서 화면에 그릴 것들을 그립니다.
		virtual void OnRender() {};

		// ImGUI 구성을 위한 함수들입니다.
		virtual void Begin() {};
		virtual void End() {};
		virtual void OnGUIRender() {};

		inline const std::string& GetName() const { return m_name; }
	};
}