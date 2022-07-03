#pragma warning (disable:4819)
#pragma once

#include <memory>

#include "Core/Base.h"
// 외부 오픈소스 라이브러리 spdlog를 이용합니다. 로깅 시스템의 추가 구현을 위한 자세한 설명과 사용법은 아래 링크를 참조하세요.
// https://github.com/gabime/spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"


namespace kepler {
	// Logger Class
	// 인스턴스 없이 전역 사용을 위해 전부 static으로 선언합니다.
	class Log
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
// 엔진(코어) 단에서 발생하는 이슈들에 대한 로그 출력에 사용합니다.
// 아래로 갈수록 (위험)강도가 높습니다.
#define KEPLER_CORE_INFO(...)		kepler::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KEPLER_CORE_TRACE(...)		kepler::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KEPLER_CORE_WARNING(...)	kepler::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KEPLER_CORE_ERROR(...)		kepler::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KEPLER_CORE_CRITICAL(...)	kepler::Log::GetCoreLogger()->critical(__VA_ARGS__)


// Client Log macros
// 클라이언트 단에서 발생하는 이슈들에 대한 로그 출력에 사용합니다.
// 아래로 갈수록 (위험)강도가 높습니다.
#define KEPLER_INFO(...)			kepler::Log::GetClientLogger()->info(__VA_ARGS__)
#define KEPLER_TRACE(...)			kepler::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KEPLER_WARNING(...)			kepler::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KEPLER_ERROR(...)			kepler::Log::GetClientLogger()->error(__VA_ARGS__)
#define KEPLER_CRITICAL(...)		kepler::Log::GetClientLogger()->critical(__VA_ARGS__)

