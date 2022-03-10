// Part of Apricot Engine. 2022-2022.
// Module: Core

#include "aepch.h"
#include "UUID.h"

#include "Random.h"

namespace Apricot {

	UUID::UUID()
		: m_UUID(Random::UInt64())
	{
	}

	UUID::UUID(uint64 uuid)
		: m_UUID(uuid)
	{
	}

}