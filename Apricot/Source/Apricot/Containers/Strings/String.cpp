// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#include "aepch.h"
#include "String.h"

#include "Apricot/Containers/Hash.h"

// TODO: Remove.
#include <string.h>

namespace Apricot {

	template<>
	APRICOT_API uint64 GetTypePtrHash(const TChar* Object)
	{
		uint64 Hash = 5381;
		int32 Ch;
	
		while (Ch = *Object++)
			Hash = ((Hash << 5) + Hash) + Ch; /* Hash * 33 + Ch */
	
		return Hash;
	}

	APRICOT_API uint64 StrLength(const TChar* String)
	{
		return wcslen(String);
	}

}