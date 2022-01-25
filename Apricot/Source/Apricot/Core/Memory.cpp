// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Memory.h"

#include "Platform.h"

namespace Apricot {

	uint64 AMemoryDebugger::AllocatedTagged[] = { 0 };
	uint64 AMemoryDebugger::FreedTagged[] = { 0 };
	uint64 AMemoryDebugger::AllocationsCountTagged[] = { 0 };
	uint64 AMemoryDebugger::DeallocationsCountTagged[] = { 0 };

	APRICOT_API void Memory_Init()
	{
		AMemoryDebugger::Init();
	}

	APRICOT_API void Memory_Destroy()
	{
		AMemoryDebugger::Destroy();
	}

	APRICOT_API void* Memory_Alloc(uint64 Size, EAllocTag Tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		AMemoryDebugger::AllocatedTagged        [(uint16)Tag] += Size;
		AMemoryDebugger::AllocationsCountTagged [(uint16)Tag] += 1;

#endif

		return APlatform::Memory_Allocate(Size, false);
	}

	APRICOT_API void Memory_Free(void* Address, uint64 Size, EAllocTag Tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		AMemoryDebugger::FreedTagged[(uint16)Tag] += Size;
		AMemoryDebugger::DeallocationsCountTagged[(uint16)Tag] += 1;

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

	void AMemoryDebugger::Init()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

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

	char8* AMemoryDebugger::GetUsageString()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		static char8 Buffer[2048] = { 0 };

		static const char8* AllocTagStrs[] =
		{
			"NONE         ",
			"UNKNOWN      ",
			"CORE         ",
			"ARRAY        ",
			"VECTOR       ",
			"STRING       ",
			"LINEAR_ALLOC ",
			"STACK_ALLOC  ",
			"DYNAMIC_ALLOC",
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
			"             - {}: {}B\n";

		Format(Buffer, AE_ARRAY_LENGTH(Buffer), Fmt,
			AllocTagStrs[(uint16)EAllocTag::None],             AllocatedTagged[(uint16)EAllocTag::None]             - FreedTagged[(uint16)EAllocTag::None],
			AllocTagStrs[(uint16)EAllocTag::Unknown],          AllocatedTagged[(uint16)EAllocTag::Unknown]          - FreedTagged[(uint16)EAllocTag::Unknown],
			AllocTagStrs[(uint16)EAllocTag::Core],             AllocatedTagged[(uint16)EAllocTag::Core]             - FreedTagged[(uint16)EAllocTag::Core],
			AllocTagStrs[(uint16)EAllocTag::Array],            AllocatedTagged[(uint16)EAllocTag::Array]            - FreedTagged[(uint16)EAllocTag::Array],
			AllocTagStrs[(uint16)EAllocTag::Vector],           AllocatedTagged[(uint16)EAllocTag::Vector]           - FreedTagged[(uint16)EAllocTag::Vector],
			AllocTagStrs[(uint16)EAllocTag::String],           AllocatedTagged[(uint16)EAllocTag::String]           - FreedTagged[(uint16)EAllocTag::String],
			AllocTagStrs[(uint16)EAllocTag::LinearAllocator],  AllocatedTagged[(uint16)EAllocTag::LinearAllocator]  - FreedTagged[(uint16)EAllocTag::LinearAllocator],
			AllocTagStrs[(uint16)EAllocTag::StackAllocator],   AllocatedTagged[(uint16)EAllocTag::StackAllocator]   - FreedTagged[(uint16)EAllocTag::StackAllocator],
			AllocTagStrs[(uint16)EAllocTag::DynamicAllocator], AllocatedTagged[(uint16)EAllocTag::DynamicAllocator] - FreedTagged[(uint16)EAllocTag::DynamicAllocator]
		);
		return Buffer;

#else
		return nullptr;
#endif
	}

}