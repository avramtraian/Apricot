// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot {

	template<typename T, typename S>
	class TPair
	{
	public:
		TPair() = default;

		template<typename TConstruct, typename SConstruct>
		TPair(TConstruct first, SConstruct second)
			: First(Forward<TConstruct>(first)), Second(Forward<SConstruct>(second)) {}

	public:
		T First;
		S Second;
	};

}