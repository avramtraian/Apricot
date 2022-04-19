#include "aepch.h"
#include "Random.h"

#include <random>

namespace Apricot {

	struct RandomData
	{
		RandomData()
			: Engine32(RandomDevice())
			, Engine64(RandomDevice())
		{}

		std::random_device RandomDevice;

		std::mt19937_64 Engine64;
		std::uniform_int_distribution<uint64_t> UniformDistribution64;

		std::mt19937 Engine32;
		std::uniform_int_distribution<uint32> UniformDistribution32;
	};
	static RandomData s_RandomData;

	uint64 Random::UInt32()
	{
		return s_RandomData.UniformDistribution32(s_RandomData.Engine32);
	}

	uint64 Random::UInt64()
	{
		return s_RandomData.UniformDistribution64(s_RandomData.Engine64);
	}

}