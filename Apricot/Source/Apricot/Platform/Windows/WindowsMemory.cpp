// Part of Apricot Engine. 2022-2022.

#include "aepch.h"

#ifdef AE_PLATFORM_WINDOWS

#include "Apricot/Core/Memory/ApricotMemory.h"

#ifdef TEXT
	#undef TEXT
#endif

#include <Windows.h>

namespace Apricot {

	void APlatformMemory::Init()
	{
	}

	void APlatformMemory::Destroy()
	{
	}

	void* APlatformMemory::Malloc(uint64 Size, uint64 Alignment)
	{
		return ::operator new(Size);
	}

	void APlatformMemory::Free(void* MemoryBlock, uint64 Size)
	{
		::operator delete(MemoryBlock, Size);
	}

	void APlatformMemory::MemCpy(void* Destination, const void* Source, uint64 SizeBytes)
	{
		memcpy(Destination, Source, SizeBytes);
	}

	void APlatformMemory::MemSet(void* Destination, int32 Value, uint64 SizeBytes)
	{
		memset(Destination, Value, SizeBytes);
	}

	void APlatformMemory::MemZero(void* Destination, uint64 SizeBytes)
	{
		memset(Destination, 0, SizeBytes);
	}

	uint64 APlatformMemory::GetAllocationSize(void* Allocation)
	{
		return _msize(Allocation);
	}

}

#endif