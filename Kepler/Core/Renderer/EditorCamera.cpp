#include "kepch.h"

#include <functional>

#include "EditorCamera.h"
#include "Core/Input.h"
#include "Core/Application.h"

namespace kepler {

	EditorCamera::EditorCamera(
		const Vec3f& position,
		const Vec3f& focalPoint,
		const float distance,
		const float fovY,
		const float aspect,
		const float nearClip,
		const float farClip
	)
		: m_pitch{ 0.0f }
		, m_yaw{ 0.0f }
		, m_lastCursorX{ 0.0f }
		, m_lastCursorY{ 0.0f }
		, m_rightInput{ 0 }
		, m_frontInput{ 0 }
		, m_upInput{ 0 }
		, PerspectiveCamera(position, focalPoint, distance, fovY, aspect, nearClip, farClip)
	{

	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		UpdateInputValue();
		Vec3f front = GetFront();
		Vec3f right = GetRight();
		Vec3f up = GetUp();

		Vec3f deltaRight = right * static_cast<float>(m_rightInput);
		Vec3f deltaUp = up * static_cast<float>(m_upInput);
		Vec3f deltaFront = front * static_cast<float>(m_frontInput);
		Vec3f deltaPos = (deltaRight + deltaUp + deltaFront) * 10.0f * deltaTime;

		m_position += deltaPos;
		m_at = m_position + (front * m_distance);
		CalcViewProjectionMatrix();

		m_rightInput = m_upInput = m_frontInput = 0;
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatch(e);

		dispatch.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorCamera::OnMouseButtonPressedEvent, this, std::placeholders::_1));
		dispatch.Dispatch<MouseMovedEvent>(std::bind(&EditorCamera::OnMouseMovedEvent, this, std::placeholders::_1));
		dispatch.Dispatch<MouseScrolledEvent>(std::bind(&EditorCamera::OnMouseScrolledEvent, this, std::placeholders::_1));
	}

	const Vec3f EditorCamera::GetFront() const
	{
		Quat ot = GetOrientation();
		// front vector(0, 1, 0)을 ot로 회전해서 그 결과 반환
		Vec3f camFront = math::Rotate(ot, kepler::Vec3f::Front);

		return camFront;
	}

	const Vec3f EditorCamera::GetUp() const
	{
		Quat ot = GetOrientation();
		// up vector(0, 1, 0)을 ot로 회전해서 그 결과 반환
		Vec3f camUp = math::Rotate(ot, kepler::Vec3f::Up);

		return camUp;
	}

	const Vec3f EditorCamera::GetRight() const
	{
		Quat ot = GetOrientation();
		// right vector(1, 0, 0)을 ot로 회전해서 그 결과 반환
		Vec3f camRight = math::Rotate(ot, kepler::Vec3f::Right);

		return camRight;
	}

	const Quat EditorCamera::GetOrientation() const
	{
		Quat orientation = Quat::FromEuler({ m_pitch, m_yaw, 0.0f });

		return orientation;
	}

	void EditorCamera::UpdateInputValue()
	{
		if (Input::IsButtonUp(mouse::Right))
		{
			return;
		}

		if (Input::IsButtonDown(key::W)) m_frontInput++;
		if (Input::IsButtonDown(key::S)) m_frontInput--;
		if (Input::IsButtonDown(key::A)) m_rightInput--;
		if (Input::IsButtonDown(key::D)) m_rightInput++;
		if (Input::IsButtonDown(key::Q)) m_upInput++;
		if (Input::IsButtonDown(key::E)) m_upInput--;
	}

	bool EditorCamera::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		if (Input::IsButtonUp(mouse::Right))
		{
			return false;
		}

		m_yaw += (e.GetX() - m_lastCursorX);
		m_pitch -= (e.GetY() - m_lastCursorY);

		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}

		m_lastCursorX = e.GetX();
		m_lastCursorY = e.GetY();

		return true;
	}

	bool EditorCamera::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		// zoom in/out
		m_fovY += e.GetVOffset() * 0.01f;
		if (m_fovY > 90.0f)
		{
			m_fovY = 90.0f;
		}
		else if (m_fovY < 5.0f)
		{
			m_fovY = 5.0f;
		}

		return true;
	}

	bool EditorCamera::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == mouse::Right)
		{
			auto& window = Application::Get()->GetWindow();
			uint32_t x, y;
			window.GetCursorPos(x, y);
			m_lastCursorX = static_cast<float>(x);
			m_lastCursorY = static_cast<float>(y);
		}

		return true;
	}
}