// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"

#include <new>

#define Memory_ZeroStruct(Variable) ::Apricot::Memory_Zero(&Variable, sizeof(Variable))
#define Memory_ZeroArray(Array) ::Apricot::Memory_Zero(Array, sizeof(Array))

namespace Apricot {

	enum class AllocTag : uint16
	{
		// Invalid allocation tag. Do not use it intentionally!
		None = 0,

		// Generic allocation
		Unknown,

		// Core engine allocations
		Core,
		Array,
		Vector,
		String,

		// Allocators
		LinearAllocator,
		StackAllocator,
		DynamicAllocator,

		// Used internally
		MaxEnumValue
	};

	APRICOT_API void Memory_Init();
	APRICOT_API void Memory_Destroy();

	APRICOT_API void* Memory_Alloc(uint64 size, AllocTag tag);

	APRICOT_API void Memory_Free(void* address, uint64 size, AllocTag tag);

	APRICOT_API void Memory_Copy(void* destination, const void* source, uint64 size);

	APRICOT_API void Memory_Set(void* destination, int32 value, uint64 size);

	APRICOT_API void Memory_Zero(void* destination, uint64 size);

	template<typename T, typename... Args>
	T* Memory_Placement(void* address, Args&&... args)
	{
		return (T*)(new (address) T(std::forward<Args>(args)...));
	}

	class APRICOT_API MemoryDebugger
	{
	public:
		static void Init();
		static void Destroy();

		static char8* GetUsageString();

	public:
		static uint64 AllocatedTagged[(uint16)AllocTag::MaxEnumValue];
		static uint64 FreedTagged[(uint16)AllocTag::MaxEnumValue];

		static uint64 AllocationsCountTagged[(uint64)AllocTag::MaxEnumValue];
		static uint64 DeallocationsCountTagged[(uint64)AllocTag::MaxEnumValue];
	};
	
}