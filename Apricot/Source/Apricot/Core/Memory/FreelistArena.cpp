// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "FreelistArena.h"

namespace Apricot {

	NODISCARD TSharedPtr<AFreelistArena> AFreelistArena::Create()
	{
		return MakeShared<AFreelistArena>();
	}

	AFreelistArena::AFreelistArena()
	{
	}

	AFreelistArena::~AFreelistArena()
	{
	}

	void AFreelistArena::GarbageCollect()
	{
		
	}

	const TChar* AFreelistArena::GetDebugName() const
	{
		return TEXT("FREELIST_ARENA");
	}

}