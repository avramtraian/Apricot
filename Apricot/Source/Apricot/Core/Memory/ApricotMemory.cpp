// Part of Apricot Engine. 2022-2022.
// Module: Memory

#include "aepch.h"
#include "ApricotMemory.h"

#include "HeapAllocator.h"

#include "Apricot/Core/Platform.h"

#include "Apricot/Profiling/MemoryProfiler.h"

namespace Apricot {

	APRICOT_API void ApricotMemoryInit()
	{
		APlatform::Init();

		if (!GMalloc)
		{
			GMalloc = (AMalloc*)APlatform::Malloc(sizeof(AMalloc), 0);
			MemConstruct<AMalloc>(GMalloc);
		}

		if (!GHeapAllocator)
		{
			GHeapAllocator = (HeapAllocator*)GMalloc->Alloc(sizeof(HeapAllocator), 0);
			MemConstruct<HeapAllocator>(GHeapAllocator);
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

		if (GHeapAllocator)
		{
			GHeapAllocator->~HeapAllocator();
			GMalloc->Free(GHeapAllocator, sizeof(HeapAllocator));
			GHeapAllocator = nullptr;
		}

		if (GMalloc)
		{
			GMalloc->~AMalloc();
			APlatform::Free(GMalloc, sizeof(AMalloc));
			GMalloc = nullptr;
		}

		APlatform::Destroy();
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

		return APlatform::Malloc(Size, Alignment);
	}

	int32 AMalloc::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		if (Size == 0)
		{
			return (int16)EMemoryError::InvalidSize;
		}
		if (!OutPointer)
		{
			return (int16)EMemoryError::InvalidOuterPointer;
		}

		*OutPointer = APlatform::Malloc(Size, Alignment);
		return (int16)EMemoryError::Success;
	}

	void* AMalloc::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		return APlatform::Malloc(Size, Alignment);;
	}

	void AMalloc::Free(void* Allocation, uint64 Size)
	{
		if (!Allocation)
		{
			return;
		}

		APlatform::Free(Allocation, Size);
	}

	int32 AMalloc::TryFree(void* Allocation, uint64 Size)
	{
		if (!Allocation)
		{
			return (int16)EMemoryError::InvalidMemoryPtr;
		}

		APlatform::Free(Allocation, Size);
		return (int16)EMemoryError::Success;
	}

	void AMalloc::FreeUnsafe(void* Allocation, uint64 Size)
	{
		APlatform::Free(Allocation, Size);
	}

	APRICOT_API void MemCpy(void* Destination, const void* Source, uint64 SizeBytes)
	{
		APlatform::MemCpy(Destination, Source, SizeBytes);
	}

	APRICOT_API void MemSet(void* Destination, int32 Value, uint64 SizeBytes)
	{
		APlatform::MemSet(Destination, Value, SizeBytes);
	}

	APRICOT_API void MemZero(void* Destination, uint64 SizeBytes)
	{
		APlatform::MemZero(Destination, SizeBytes);
	}

}