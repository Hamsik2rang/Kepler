#pragma once

extern kepler::Application* kepler::CreateApplication();

int main(int argc, char** argv)
{
	auto app = kepler::CreateApplication();
	app->Run();
	delete app;

	return 0;
}