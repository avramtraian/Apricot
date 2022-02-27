// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

namespace Apricot {

	APRICOT_API uint64 StrLength(const TChar* String);

	class APRICOT_API AString
	{
	public:
		AString();

		AString(const TChar* Other);
	};

}