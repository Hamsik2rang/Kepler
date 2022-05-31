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
		// �� ���� Ÿ�̸�
		inline float Elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - m_timePoint).count() * 0.001f * 0.001f * 0.001f; }
		// �и��� ���� Ÿ�̸�
		inline float ElapsedMills() { return Elapsed() * 1000.0f; }
	};
}