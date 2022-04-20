#include "aepch.h"
#include "UUID.h"

#include "Random.h"

namespace Apricot {

	UUID::UUID()
		: m_UUID(Random::UInt64())
	{
	}

	UUID::UUID(uint64 Uuid)
		: m_UUID(Uuid)
	{
	}

	UUID& UUID::operator=(uint64 Uuid)
	{
		m_UUID = Uuid;
		return *this;
	}

}