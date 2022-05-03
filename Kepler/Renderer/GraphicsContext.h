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
		// TODO: 추후 API사양을 인자로 받아 적절한 컨텍스트를 생성하도록 변경할 것(현재는 DX11만 생성 가능)
		static GraphicsContext* Create(HWND hWnd);
	};
}