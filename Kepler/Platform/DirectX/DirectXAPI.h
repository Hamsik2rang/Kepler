#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace kepler {

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

}