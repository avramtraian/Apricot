// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "ApricotMemory.h"

#include "Apricot/Profiling/MemoryProfiler.h"

namespace Apricot {

	APRICOT_API void ApricotMemoryInit()
	{
		APlatformMemory::Init();

		if (!GMalloc)
		{
			GMalloc = (AMalloc*)APlatformMemory::Malloc(sizeof(AMalloc), 0);
			MemConstruct<AMalloc>(GMalloc);
		}

	#ifdef AE_ENABLE_MEMORY_TRACE
		AMemoryProfiler::Init();
	#endif
	}

	APRICOT_API void ApricotMemoryDestroy()
	{
	#ifdef AE_ENABLE_MEMORY_TRACE
		AMemoryProfiler::Destroy();
	#endif

		if (GMalloc)
		{
			GMalloc->~AMalloc();
			APlatformMemory::Free(GMalloc, sizeof(AMalloc));
			GMalloc = nullptr;
		}

		APlatformMemory::Destroy();
	}

	APRICOT_API extern AMalloc* GMalloc = nullptr;

	AMalloc::AMalloc()
	{
	}

	AMalloc::~AMalloc()
	{
	}

	void* AMalloc::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*) */)
	{
		if (Size == 0)
		{
			return nullptr;
		}

		return APlatformMemory::Malloc(Size, Alignment);
	}

	int32 AMalloc::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (Size == 0)
		{
			return AE_ALLOC_INVALID_SIZE;
		}
		if (!OutPointer)
		{
			return AE_ALLOC_INVALID_PARAM;
		}

		*OutPointer = APlatformMemory::Malloc(Size, Alignment);
		return AE_ALLOC_SUCCESSFULLY;
	}

	void* AMalloc::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		return APlatformMemory::Malloc(Size, Alignment);;
	}

	void AMalloc::Free(void* Allocation, uint64 Size)
	{
		if (!Allocation)
		{
			return;
		}

		APlatformMemory::Free(Allocation, Size);
	}

	int32 AMalloc::TryFree(void* Allocation, uint64 Size)
	{
		if (!Allocation)
		{
			return AE_FREE_BAD_POINTER;
		}

		APlatformMemory::Free(Allocation, Size);
		return AE_FREE_SUCCESSFULLY;
	}

	void AMalloc::FreeUnsafe(void* Allocation, uint64 Size)
	{
		APlatformMemory::Free(Allocation, Size);
	}

	APRICOT_API void MemCpy(void* Destination, const void* Source, uint64 SizeBytes)
	{
		APlatformMemory::MemCpy(Destination, Source, SizeBytes);
	}

	APRICOT_API void MemSet(void* Destination, int32 Value, uint64 SizeBytes)
	{
		APlatformMemory::MemSet(Destination, Value, SizeBytes);
	}

	APRICOT_API void MemZero(void* Destination, uint64 SizeBytes)
	{
		APlatformMemory::MemZero(Destination, SizeBytes);
	}

}