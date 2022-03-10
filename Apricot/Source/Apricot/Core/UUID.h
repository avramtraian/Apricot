// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API UUID
	{
	public:
		UUID();
		UUID(uint64 uuid);

	public:
		operator uint64() const { return m_UUID; }

	private:
		uint64 m_UUID;
	};

}