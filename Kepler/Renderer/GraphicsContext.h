#pragma once

#include <memory>
#include <Windows.h>
#include "Core/Base.h"
#include "Renderer/RenderAPI.h"

namespace kepler {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() {}
		virtual bool Init() = 0;
		virtual void Cleanup() = 0;
		// swap buffer
		virtual void SwapBuffer(bool bVSync = true) = 0;
		// TODO: ���� API����� ���ڷ� �޾� ������ ���ؽ�Ʈ�� �����ϵ��� ������ ��(����� DX11�� ���� ����)
		static GraphicsContext* Create(HWND hWnd);
	};
}