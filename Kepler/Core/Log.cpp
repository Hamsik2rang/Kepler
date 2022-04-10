#include "kepch.h"

#include "Log.h"

namespace kepler {
	std::shared_ptr<spdlog::logger> Log::s_pCoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_pClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_pCoreLogger = spdlog::stdout_color_mt("KEPLER");
		s_pCoreLogger->set_level(spdlog::level::trace);

		s_pClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_pClientLogger->set_level(spdlog::level::trace);
	}
}