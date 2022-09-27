#pragma once

#include <memory>
#include <Windows.h>
#include "Core/Base.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Platform/Windows/WindowsWindow.h"

namespace kepler {

	struct WindowData;

	class IGraphicsContext
	{
	private:
		static IGraphicsContext* s_pInstance;

	public:
		virtual ~IGraphicsContext();
		virtual bool Init(const WindowData& data) = 0;
		virtual void Cleanup() = 0;
		virtual void SwapBuffer() = 0;

		// Interface for DirectX11 
		virtual ID3D11Device* GetDevice() { return nullptr; }
		virtual ID3D11DeviceContext* GetDeviceContext() { return nullptr; }
		virtual IDXGISwapChain* GetSwapChain() { return nullptr; }

		// TODO: ���� API����� ���ڷ� �޾� ������ ���ؽ�Ʈ�� �����ϵ��� ������ ��(����� DX11�� ���� ����)
		static IGraphicsContext* Create(const void* WindowHandle);

		inline static IGraphicsContext* Get() { return s_pInstance; }
	};
}