// Part of Apricot Engine. 2022-2022.
// Module: Core

#include "aepch.h"
#include "Random.h"

#include <random>

namespace Apricot {

	static std::random_device SRandomDevice;
	static std::mt19937_64 SEngine(SRandomDevice());
	static std::uniform_int_distribution<uint64_t> SUniformDistribution;

	uint64 Random::UInt64()
	{
		return SUniformDistribution(SEngine);
	}

}