#pragma once

#include "Base.h"

#include <type_traits>
#include <new>

namespace Apricot { namespace Memory {

	enum class AllocTag : uint32
	{
		None = 0,
		General,
		CoreSystems,
		LinearAllocator,
		StackAllocator,

		MaxEnum
	};

	struct APRICOT_API HMemoryDebugger
	{
		static void DebugLog();
		static void DebugLogAllocationSizes();
		static void DebugLogAllocatiosCount();

		static uint64 AllocationSizes[(uint32)AllocTag::MaxEnum];
		static uint64 AllocationsCount[(uint32)AllocTag::MaxEnum];
	};

	inline void* Allocate(uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::AllocationSizes[(uint32)tag] += size;
		HMemoryDebugger::AllocationsCount[(uint32)tag]++;
#endif

		return ::operator new(size);
	}

	inline void Free(void* block, uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::AllocationSizes[(uint32)tag] -= size;
		HMemoryDebugger::AllocationsCount[(uint32)tag]--;
#endif

		return ::operator delete(block, size);
	}

	template<typename T, typename... Args>
	inline T* New(AllocTag tag, Args&&... args)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::AllocationSizes[(uint32)tag] += sizeof(T);
		HMemoryDebugger::AllocationsCount[(uint32)tag]++;
#endif

		return new T(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline T* PlacementNew(AllocTag tag, void* address, Args&&... args)
	{
		return new(address) T(std::forward<Args>(args)...);
	}

	template<typename T>
	inline void Delete(T* block, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		// This data might not be real
		HMemoryDebugger::AllocationSizes[(uint32)tag] -= sizeof(T);
		HMemoryDebugger::AllocationsCount[(uint32)tag]--;
#endif

		delete block;
	}

	template<typename T>
	inline void Delete(T* block, uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::AllocationSizes[(uint32)tag] -= size;
		HMemoryDebugger::AllocationsCount[(uint32)tag]--;
#endif

		delete block;
	}

	inline void Copy(void* dest, const void* src, uint64 size)
	{
		memcpy(dest, src, size);
	}

	inline void SetZero(void* block, uint64 size)
	{
		memset(block, 0, size);
	}

	inline void Set(void* block, uint64 size, int value)
	{
		memset(block, value, size);
	}

} }