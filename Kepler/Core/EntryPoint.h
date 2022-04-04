#pragma once

#ifdef KEPLER_PLATFORM_WINDOWS

extern kepler::Application* kepler::CreateApplication();

int main(int argc, char** argv)
{
	printf("Kepler Engine\n");
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