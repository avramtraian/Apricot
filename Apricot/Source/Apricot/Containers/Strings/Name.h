// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#include "String.h"

namespace Apricot {

	class APRICOT_API AName
	{
	public:
		AName(const TChar* String);

	private:
		uint64 m_Index;
	};

}