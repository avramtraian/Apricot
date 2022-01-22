#include "aepch.h"
#include "Memory.h"

#include "Platform.h"

namespace Apricot {

	uint64 MemoryDebugger::AllocatedTagged[] = { 0 };
	uint64 MemoryDebugger::FreedTagged[] = { 0 };
	uint64 MemoryDebugger::AllocationsCountTagged[] = { 0 };
	uint64 MemoryDebugger::DeallocationsCountTagged[] = { 0 };

	APRICOT_API void Memory_Init()
	{
		MemoryDebugger::Init();
	}

	APRICOT_API void Memory_Destroy()
	{
		MemoryDebugger::Destroy();
	}

	APRICOT_API void* Memory_Alloc(uint64 size, AllocTag tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		MemoryDebugger::AllocatedTagged        [(uint16)tag] += size;
		MemoryDebugger::AllocationsCountTagged [(uint16)tag] += 1;

#endif

		return Platform::Memory_Allocate(size, false);
	}

	APRICOT_API void Memory_Free(void* address, uint64 size, AllocTag tag)
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		MemoryDebugger::FreedTagged[(uint16)tag] += size;
		MemoryDebugger::DeallocationsCountTagged[(uint16)tag] += 1;

#endif

		Platform::Memory_Free(address, size);
	}

	APRICOT_API void Memory_Copy(void* destination, const void* source, uint64 size)
	{
		Platform::Memory_Copy(destination, source, size);
	}

	APRICOT_API void Memory_Set(void* destination, int32 value, uint64 size)
	{
		Platform::Memory_Set(destination, value, size);
	}

	APRICOT_API void Memory_Zero(void* destination, uint64 size)
	{
		Platform::Memory_Zero(destination, size);
	}

	void MemoryDebugger::Init()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		Memory_ZeroArray(AllocatedTagged);
		Memory_ZeroArray(FreedTagged);

		Memory_ZeroArray(AllocationsCountTagged);
		Memory_ZeroArray(DeallocationsCountTagged);

#endif
	}

	void MemoryDebugger::Destroy()
	{
#ifdef AE_ENABLE_MEMORY_TRACE
#endif
	}

	char8* MemoryDebugger::GetUsageString()
	{
#ifdef AE_ENABLE_MEMORY_TRACE

		static char8 buffer[2048] = { 0 };

		static const char8* allocTagStrs[] =
		{
			"NONE   ",
			"UNKNOWN",
			"CORE   ",
		};

		static const char8* format =
			"Tagged Memory Usage:\n"
			"    - {}: {}B\n"
			"    - {}: {}B\n"
			"    - {}: {}B\n";

		Format(format, buffer, AE_ARRAY_LENGTH(buffer), 
			allocTagStrs[(uint16)AllocTag::None],    AllocatedTagged[(uint16)AllocTag::None]    - FreedTagged[(uint16)AllocTag::None],
			allocTagStrs[(uint16)AllocTag::Unknown], AllocatedTagged[(uint16)AllocTag::Unknown] - FreedTagged[(uint16)AllocTag::Unknown],
			allocTagStrs[(uint16)AllocTag::Core],    AllocatedTagged[(uint16)AllocTag::Core]    - FreedTagged[(uint16)AllocTag::Core]
		);
		return buffer;

#else
		return nullptr;
#endif
	}

}