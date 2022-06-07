#pragma once


#include "Core/Base.h"
#include "Core/Layer.hpp"

#include "Core/Event/KeyEvent.hpp"
#include "Core/Event/MouseEvent.hpp"
#include "Core/Event/ApplicationEvent.hpp"


namespace kepler {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Event& e) override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void OnGUIRender() override;


		static LRESULT ImGuiEventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}