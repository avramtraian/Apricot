#pragma once

#include "Base.h"

#include <new>

#define Memory_ZeroStruct(Variable) ::Apricot::Memory_Zero(&Variable, sizeof(Variable))
#define Memory_ZeroArray(Variable) ::Apricot::Memory_Zero(Variable, sizeof(Variable))

namespace Apricot {

	enum class AllocTag : uint16
	{
		None = 0,

		Unknown,
		Core,

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