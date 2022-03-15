// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot {

	enum class EAllocatorType : uint16
	{
		None = 0,

		Heap,

		MaxEnumValue
	};

	enum class EAllocatorHint : uint16
	{
		None = 0,

		Vector,
		String,
		HashMap,

		MaxEnumValue
	};

}