#pragma once

#include <random>
#include <numeric>
#include "Core/Base.h"

namespace kepler {

	class Random
	{
	private:
		static std::mt19937_64 s_engine;

	public:
		static void Init()
		{
			s_engine.seed(std::random_device()());
		}
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max
		static float Float(float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max())
		{
			std::uniform_real_distribution<> dis(min, max);
			float gen = static_cast<float>(dis(s_engine));

			return gen;
		}

		static double Double(double min = std::numeric_limits<double>::min(), double max = std::numeric_limits<double>::max())
		{
			std::uniform_real_distribution<> dis(min, max);
			double gen = static_cast<double>(dis(s_engine));

			return gen;
		}

		static int32_t Int32(int32_t min = std::numeric_limits<int32_t>::min(), int32_t max = std::numeric_limits<int32_t>::max())
		{
			std::uniform_int_distribution<int32_t> dis(min, max);
			int32_t gen = dis(s_engine);

			return gen;
		}

		static uint32_t Uint32(uint32_t min = std::numeric_limits<uint32_t>::min(), uint32_t max = std::numeric_limits<uint32_t>::max())
		{
			std::uniform_int_distribution<uint32_t> dis(min, max);
			uint32_t gen = dis(s_engine);

			return gen;
		}

		static int64_t Int64(int64_t min = std::numeric_limits<int64_t>::min(), int64_t max = std::numeric_limits<int64_t>::max())
		{
			std::uniform_int_distribution<int64_t> dis(min, max);
			int64_t gen = dis(s_engine);

			return gen;
		}

		static uint64_t Uint64(uint64_t min = std::numeric_limits<uint64_t>::min(), uint64_t max = std::numeric_limits<uint64_t>::max())
		{
			std::uniform_int_distribution<uint64_t> dis(min, max);
			uint64_t gen = dis(s_engine);

			return gen;
		}

#pragma pop_macro("max")
#pragma pop_macro("min")
	};
}