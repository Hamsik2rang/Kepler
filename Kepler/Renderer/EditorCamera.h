#pragma once

#include "PerspectiveCamera.h"
#include "Core/Base.h"
#include "Core/Event/Event.hpp"
#include "Core/Event/MouseEvent.hpp"
#include "Core/Event/KeyEvent.hpp"

namespace kepler {

	class EditorCamera : public PerspectiveCamera
	{
	private:
		float m_pitch;
		float m_yaw;

		float m_lastCursorX;
		float m_lastCursorY;

		int m_rightInput;
		int m_frontInput;
		int m_upInput;

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		void UpdatePosition();

	public:
		EditorCamera() = default;
		EditorCamera(const Vec3f& position, const Vec3f& focalPoint, const float distance, const float fovY, const float aspect, const float nearClip, const float farClip);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		const Vec3f GetFront() const;
		const Vec3f GetUp() const;
		const Vec3f GetRight() const;
		const Quat GetOrientation() const;

		inline void SetPitch(float pitch) { m_pitch = pitch; }
		inline void SetYaw(float yaw) { m_yaw = yaw; }
		inline float GetPitch() const { return m_pitch; }
		inline float GetYaw() const { return m_yaw; }
	};
}
