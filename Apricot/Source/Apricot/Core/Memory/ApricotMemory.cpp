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

	APRICOT_API void* AMalloc::Alloc(uint64 Size)
	{
		if (Size == 0)
		{
			return nullptr;
		}

		return APlatformMemory::Malloc(Size, 0);
	}

	APRICOT_API void AMalloc::Free(void* Allocation)
	{
		if (!Allocation)
		{
			return;
		}

		APlatformMemory::Free(Allocation, 0);
	}

	APRICOT_API void* AMalloc::Alloc(uint64 Size, const TChar* File, const TChar* Func, uint64 Line)
	{
		void* Allocation = Alloc(Size);
	#ifdef AE_ENABLE_MEMORY_TRACE
		GMemoryProfiler->SubmitLowLevelMalloc(Size, File, Func, Line);
	#endif
		return Allocation;
	}

	APRICOT_API void AMalloc::Free(void* Allocation, const TChar* File, const TChar* Func, uint64 Line)
	{
	#ifdef AE_ENABLE_MEMORY_TRACE
		uint64 AllocationSize = APlatformMemory::GetAllocationSize(Allocation);
		GMemoryProfiler->SubmitLowLevelFree(AllocationSize, File, Func, Line);
	#endif
		Free(Allocation);
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