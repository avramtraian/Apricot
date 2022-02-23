// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#define NULL_SHARED ::Apricot::NullPlaceholder()
#define NULL_WEAK   ::Apricot::NullPlaceholder()
#define NULL_UNIQUE ::Apricot::NullPlaceholder()

namespace Apricot {

	struct NullPlaceholder
	{
		uint8 Data;
	};

}