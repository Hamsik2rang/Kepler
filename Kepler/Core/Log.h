#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace kepler {

	class KEPLER_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_pCoreLogger;
		static std::shared_ptr<spdlog::logger> s_pClientLogger;

	public:
		static void Init();
		
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_pCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_pClientLogger; }
	};
}

// Core Log macros
#define KEPLER_CORE_ERROR(...)		::kepler::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KEPLER_CORE_WARNING(...)	::kepler::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KEPLER_CORE_INFO(...)		::kepler::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KEPLER_CORE_TRACE(...)		::kepler::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KEPLER_CORE_FATAL(...)		::kepler::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log macros
#define KEPLER_CLIENT_ERROR(...)	::kepler::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KEPLER_CLIENT_WARNING(...)	::kepler::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KEPLER_CLIENT_INFO(...)		::kepler::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KEPLER_CLIENT_TRACE(...)	::kepler::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KEPLER_CLIENT_FATAL(...)	::kepler::Log::GetCoreLogger()->fatal(__VA_ARGS__)

