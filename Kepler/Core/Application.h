#pragma once

#include "Core.h"

namespace kepler {

	class KEPLER_API Application
	{
	public:
		Application();
		~Application();

		void Run();
	};

	Application* CreateApplication();
}