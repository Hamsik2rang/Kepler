#pragma once

#include "Core/Base.h"
#include "Core/Layer.hpp"

#include "Core/Event/KeyEvent.hpp"
#include "Core/Event/MouseEvent.hpp"
#include "Core/Event/ApplicationEvent.hpp"


namespace kepler {

	class KEPLER_API ImGuiLayer : public Layer
	{
	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
	};

}