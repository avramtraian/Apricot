// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"

#include <new>

#define Memory_ZeroStruct(Variable) ::Apricot::Memory_Zero(&Variable, sizeof(Variable))
#define Memory_ZeroArray(Array) ::Apricot::Memory_Zero(Array, sizeof(Array))

namespace Apricot {

	enum class EAllocRegion
	{
		// Invalid allocation regions.
		None = 0,
		Unknown,

		// Valid allocation regions.
		Stack,
		Heap,
	};

	enum class EAllocSubregion
	{
		// Invalid allocation subregion.
		None = 0,
		
		// Default allocation subregion.
		Unknown,

		// Memory subregion managed by the allocators.
		HeapAllocator,
		LinearAllocator,
		StackAllocator,
		DynamicAllocator
	};

	enum class EAllocTag : uint16
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

		SharedPtr,
		UniquePtr,

		// Used internally
		MaxEnumValue
	};

	APRICOT_API void Memory_Init();
	APRICOT_API void Memory_Destroy();

	APRICOT_API void* Memory_Alloc(uint64 Size, EAllocTag Tag);

	APRICOT_API void Memory_Free(void* Address, uint64 Size, EAllocTag Tag);

	APRICOT_API void Memory_Copy(void* Destination, const void* Source, uint64 Size);

	APRICOT_API void Memory_Set(void* Destination, int32 Value, uint64 Size);

	APRICOT_API void Memory_Zero(void* Destination, uint64 Size);

	template<typename T, typename... Args>
	T* Memory_Placement(void* Address, Args&&... args)
	{
		return (T*)(new (Address) T(std::forward<Args>(args)...));
	}

	class APRICOT_API AMemoryDebugger
	{
	public:
		static void Init();
		static void Destroy();

		static char8* GetUsageString();

	public:
		static uint64 AllocatedTagged[(uint16)EAllocTag::MaxEnumValue];
		static uint64 FreedTagged[(uint16)EAllocTag::MaxEnumValue];

		static uint64 AllocationsCountTagged[(uint64)EAllocTag::MaxEnumValue];
		static uint64 DeallocationsCountTagged[(uint64)EAllocTag::MaxEnumValue];
	};
	
}