// Entry Point
#pragma once

#ifdef KEPLER_PLATFORM_WINDOWS

extern kepler::Application* kepler::CreateApplication();

#ifdef KEPLER_GRAPHICS_API_DIRECTX
#include <Windows.h>

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int	nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	kepler::g_hInst = hInstance;
	kepler::g_nCmdShow = nCmdShow;

	//----- Initialize Core resources -----
	kepler::Log::Init();
	kepler::Audio::Init();

	//-------------------------------------

	int year = 2022;
	int month = 4;
	int day = 4;
	KEPLER_CORE_WARNING("Initializing Log System");
	KEPLER_CORE_INFO("Welcome to Kepler Engine, this code written in {0}/{1:02}/{2:02}", year, month, day);
	
	auto app = kepler::CreateApplication();
	app->Run();
	

	delete app;
	//----- Release Core resources -----
	kepler::Audio::Release();
	//----------------------------------

	return 0;
}

#else
int main(int argc, char** argv)
{
	kepler::Log::Init();

	int year = 2022;
	int month = 4;
	int day = 4;
	KEPLER_CORE_WARNING("Initializing Log System");
	KEPLER_CORE_INFO("Welcome to Kepler Engine, this code written in {0}/{1:02}/{2:02}", year, month, day);

	auto app = kepler::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif

#endif