#pragma warning (disable:4819)
#pragma once

#include <memory>

#include "Core/Base.h"
// �ܺ� ���¼ҽ� ���̺귯�� spdlog�� �̿��մϴ�. �α� �ý����� �߰� ������ ���� �ڼ��� ����� ������ �Ʒ� ��ũ�� �����ϼ���.
// https://github.com/gabime/spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"


namespace kepler {
	// Logger Class
	// �ν��Ͻ� ���� ���� ����� ���� ���� static���� �����մϴ�.
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
// ����(�ھ�) �ܿ��� �߻��ϴ� �̽��鿡 ���� �α� ��¿� ����մϴ�.
// �Ʒ��� ������ (����)������ �����ϴ�.
#define KEPLER_CORE_INFO(...)		kepler::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KEPLER_CORE_TRACE(...)		kepler::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KEPLER_CORE_WARNING(...)	kepler::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KEPLER_CORE_ERROR(...)		kepler::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KEPLER_CORE_CRITICAL(...)	kepler::Log::GetCoreLogger()->critical(__VA_ARGS__)


// Client Log macros
// Ŭ���̾�Ʈ �ܿ��� �߻��ϴ� �̽��鿡 ���� �α� ��¿� ����մϴ�.
// �Ʒ��� ������ (����)������ �����ϴ�.
#define KEPLER_INFO(...)			kepler::Log::GetClientLogger()->info(__VA_ARGS__)
#define KEPLER_TRACE(...)			kepler::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KEPLER_WARNING(...)			kepler::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KEPLER_ERROR(...)			kepler::Log::GetClientLogger()->error(__VA_ARGS__)
#define KEPLER_CRITICAL(...)		kepler::Log::GetClientLogger()->critical(__VA_ARGS__)

