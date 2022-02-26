// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

namespace Apricot {

	template<typename T>
	APRICOT_API uint64 GetTypeHash(const T& Object);

	template<typename T>
	APRICOT_API uint64 GetTypePtrHash(T* Object);

}