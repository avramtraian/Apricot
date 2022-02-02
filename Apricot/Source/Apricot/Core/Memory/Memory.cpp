// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Memory.h"

#include "Apricot/Core/Platform.h"

#include "HeapAllocator.h"

namespace Apricot {

	APRICOT_API void Memory_Init()
	{
		AMemoryDebugger::Init();

		AHeapAllocator::Init();
	}

	APRICOT_API void Memory_Destroy()
	{
		AHeapAllocator::Destroy();

		AMemoryDebugger::Destroy();
	}

	APRICOT_API void* Memory_Alloc(uint64 Size, EAllocSubregion Subregion)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		AMemoryDebugger::AllocatedRegions           [(uint8)EAllocRegion::Heap] += Size;
		AMemoryDebugger::AllocationsCountRegions    [(uint8)EAllocRegion::Heap] += 1;
												    
		AMemoryDebugger::AllocatedSubregions        [(uint16)Subregion] += Size;
		AMemoryDebugger::AllocationsCountSubregions [(uint16)Subregion] += 1;

#endif

		return APlatform::Memory_Allocate(Size, false);
	}

	APRICOT_API void Memory_Free(void* Address, uint64 Size, EAllocSubregion Subregion)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		AMemoryDebugger::FreedRegions                 [(uint8)EAllocRegion::Heap] += Size;
		AMemoryDebugger::DeallocationsCountRegions    [(uint8)EAllocRegion::Heap] += 1;

		AMemoryDebugger::FreedSubregions              [(uint16)Subregion] += Size;
		AMemoryDebugger::DeallocationsCountSubregions [(uint16)Subregion] += 1;

#endif

		APlatform::Memory_Free(Address, Size);
	}

	APRICOT_API void Memory_Copy(void* Destination, const void* Source, uint64 Size)
	{
		APlatform::Memory_Copy(Destination, Source, Size);
	}

	APRICOT_API void Memory_Set(void* Destination, int32 Value, uint64 Size)
	{
		APlatform::Memory_Set(Destination, Value, Size);
	}

	APRICOT_API void Memory_Zero(void* Destination, uint64 Size)
	{
		APlatform::Memory_Zero(Destination, Size);
	}





	//////////////////////////////////////////////////////////////////////////////
	////////////////////////// MEMORY DEBUGGER ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	void AMemoryDebugger::Init()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		Memory_ZeroArray(AllocatedRegions);
		Memory_ZeroArray(FreedRegions);

		Memory_ZeroArray(AllocationsCountRegions);
		Memory_ZeroArray(DeallocationsCountRegions);

		Memory_ZeroArray(AllocatedSubregions);
		Memory_ZeroArray(FreedSubregions);

		Memory_ZeroArray(AllocationsCountSubregions);
		Memory_ZeroArray(DeallocationsCountSubregions);

		Memory_ZeroArray(AllocatedTagged);
		Memory_ZeroArray(FreedTagged);

		Memory_ZeroArray(AllocationsCountTagged);
		Memory_ZeroArray(DeallocationsCountTagged);

#endif
	}

	void AMemoryDebugger::Destroy()
	{
#ifdef AE_ENABLE_MEMORY_TRACE
#endif
	}

	char8* AMemoryDebugger::GetRegionsUsageString()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		static char8 Buffer[2048] = { 0 };

		static const char8* AllocRegionStrs[] =
		{
			"NONE         ",
			"UNKNOWN      ",
			"HEAP         ",
			"STACK        ",
		};

		static const char8* Fmt =
			"Regions Memory Usage:\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n";

		Format(Buffer, AE_ARRAY_LENGTH(Buffer), Fmt,
			AllocRegionStrs[(uint8)EAllocRegion::None],    AllocatedRegions[(uint8)EAllocRegion::None]    - FreedRegions[(uint8)EAllocRegion::None],
			AllocRegionStrs[(uint8)EAllocRegion::Unknown], AllocatedRegions[(uint8)EAllocRegion::Unknown] - FreedRegions[(uint8)EAllocRegion::Unknown],
			AllocRegionStrs[(uint8)EAllocRegion::Heap],    AllocatedRegions[(uint8)EAllocRegion::Heap]    - FreedRegions[(uint8)EAllocRegion::Heap],
			AllocRegionStrs[(uint8)EAllocRegion::Stack],   AllocatedRegions[(uint8)EAllocRegion::Stack]   - FreedRegions[(uint8)EAllocRegion::Stack]
		);
		return Buffer;

#else
		return nullptr;
#endif
	}

	char8* AMemoryDebugger::GetSubregionsUsageString()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		static char8 Buffer[2048] = { 0 };

		static const char8* AllocTagStrs[] =
		{
			"NONE         ",
			"UNKNOWN      ",
			"HEAP_ALLOC   ",
			"LINEAR_ALLOC ",
			"STACK_ALLOC  ",
			"DYNAMIC_ALLOC",
		};

		static const char8* Fmt =
			"Subregions Memory Usage:\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n";

		Format(Buffer, AE_ARRAY_LENGTH(Buffer), Fmt,
			AllocTagStrs[(uint16)EAllocSubregion::None],             AllocatedSubregions[(uint16)EAllocSubregion::None]             - FreedSubregions[(uint16)EAllocSubregion::None],
			AllocTagStrs[(uint16)EAllocSubregion::Unknown],          AllocatedSubregions[(uint16)EAllocSubregion::Unknown]          - FreedSubregions[(uint16)EAllocSubregion::Unknown],
			AllocTagStrs[(uint16)EAllocSubregion::HeapAllocator],    AllocatedSubregions[(uint16)EAllocSubregion::HeapAllocator]    - FreedSubregions[(uint16)EAllocSubregion::HeapAllocator],
			AllocTagStrs[(uint16)EAllocSubregion::LinearAllocator],  AllocatedSubregions[(uint16)EAllocSubregion::LinearAllocator]  - FreedSubregions[(uint16)EAllocSubregion::LinearAllocator],
			AllocTagStrs[(uint16)EAllocSubregion::StackAllocator],   AllocatedSubregions[(uint16)EAllocSubregion::StackAllocator]   - FreedSubregions[(uint16)EAllocSubregion::StackAllocator],
			AllocTagStrs[(uint16)EAllocSubregion::DynamicAllocator], AllocatedSubregions[(uint16)EAllocSubregion::DynamicAllocator] - FreedSubregions[(uint16)EAllocSubregion::DynamicAllocator]
		);
		return Buffer;

#else
		return nullptr;
#endif
	}

	char8* AMemoryDebugger::GetTaggedUsageString()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		static char8 Buffer[2048] = { 0 };

		static const char8* AllocTagStrs[] =
		{
			"NONE         ",
			"UNKNOWN      ",
			"CORE         ",
			"DEBUG        ",
			"ARRAY        ",
			"VECTOR       ",
			"STRING       ",
			"LINEAR_ALLOC ",
			"STACK_ALLOC  ",
			"DYNAMIC_ALLOC",
			"SHARED_PTR   ",
			"UNIQUE_PTR   ",
		};

		static const char8* Fmt =
			"Tagged Memory Usage:\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n"
			"             - {}: {}B\n";

		Format(Buffer, AE_ARRAY_LENGTH(Buffer), Fmt,
			AllocTagStrs[(uint16)EAllocTag::None],             AllocatedTagged[(uint16)EAllocTag::None]             - FreedTagged[(uint16)EAllocTag::None],
			AllocTagStrs[(uint16)EAllocTag::Unknown],          AllocatedTagged[(uint16)EAllocTag::Unknown]          - FreedTagged[(uint16)EAllocTag::Unknown],
			AllocTagStrs[(uint16)EAllocTag::Core],             AllocatedTagged[(uint16)EAllocTag::Core]             - FreedTagged[(uint16)EAllocTag::Core],
			AllocTagStrs[(uint16)EAllocTag::Debug],            AllocatedTagged[(uint16)EAllocTag::Debug]            - FreedTagged[(uint16)EAllocTag::Debug],
			AllocTagStrs[(uint16)EAllocTag::Array],            AllocatedTagged[(uint16)EAllocTag::Array]            - FreedTagged[(uint16)EAllocTag::Array],
			AllocTagStrs[(uint16)EAllocTag::Vector],           AllocatedTagged[(uint16)EAllocTag::Vector]           - FreedTagged[(uint16)EAllocTag::Vector],
			AllocTagStrs[(uint16)EAllocTag::String],           AllocatedTagged[(uint16)EAllocTag::String]           - FreedTagged[(uint16)EAllocTag::String],
			AllocTagStrs[(uint16)EAllocTag::LinearAllocator],  AllocatedTagged[(uint16)EAllocTag::LinearAllocator]  - FreedTagged[(uint16)EAllocTag::LinearAllocator],
			AllocTagStrs[(uint16)EAllocTag::StackAllocator],   AllocatedTagged[(uint16)EAllocTag::StackAllocator]   - FreedTagged[(uint16)EAllocTag::StackAllocator],
			AllocTagStrs[(uint16)EAllocTag::DynamicAllocator], AllocatedTagged[(uint16)EAllocTag::DynamicAllocator] - FreedTagged[(uint16)EAllocTag::DynamicAllocator],
			AllocTagStrs[(uint16)EAllocTag::SharedPtr],        AllocatedTagged[(uint16)EAllocTag::SharedPtr]        - FreedTagged[(uint16)EAllocTag::SharedPtr],
			AllocTagStrs[(uint16)EAllocTag::UniquePtr],        AllocatedTagged[(uint16)EAllocTag::UniquePtr]        - FreedTagged[(uint16)EAllocTag::UniquePtr]
		);
		return Buffer;

#else
		return nullptr;
#endif
	}

	uint64 AMemoryDebugger::DeallocationsCountTagged[] = { 0 };

	uint64 AMemoryDebugger::AllocationsCountTagged[] = { 0 };

	uint64 AMemoryDebugger::FreedTagged[] = { 0 };

	uint64 AMemoryDebugger::AllocatedTagged[] = { 0 };

	uint64 AMemoryDebugger::DeallocationsCountSubregions[] = { 0 };

	uint64 AMemoryDebugger::AllocationsCountSubregions[] = { 0 };

	uint64 AMemoryDebugger::FreedSubregions[] = { 0 };

	uint64 AMemoryDebugger::AllocatedSubregions[] = { 0 };

	uint64 AMemoryDebugger::DeallocationsCountRegions[] = { 0 };

	uint64 AMemoryDebugger::AllocationsCountRegions[] = { 0 };

	uint64 AMemoryDebugger::FreedRegions[] = { 0 };

	uint64 AMemoryDebugger::AllocatedRegions[] = { 0 };

}