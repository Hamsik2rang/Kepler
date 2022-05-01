#pragma once

#include <chrono>

namespace kepler {
	
	class Timer
	{
		using Clock = std::chrono::high_resolution_clock;
	private:
		std::chrono::time_point<Clock> m_timePoint;

	public:
		Timer() { Init(); }
		~Timer() {}

		inline void Init() { m_timePoint = Clock::now(); }
		inline void Start() { Init(); }
		// 초 단위 타이머
		inline float Elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - m_timePoint).count() * 0.001f * 0.001f * 0.001f; }
		// 밀리초 단위 타이머
		inline float ElapsedMills() { return Elapsed() * 1000.0f; }
	};
}