#pragma once

#include "Base.h"

#include <type_traits>
#include <new>

/*
* Converts that unit into bytes.
*/
#define AE_KILOBYTES(K) K * 1024
#define AE_MEGABYTES(M) M * 1024 * 1024
#define AE_GIGABYTES(G) G * 1024 * 1024 * 1024

namespace Apricot { namespace Memory {

	enum class AllocTag : uint32
	{
		None = 0,
		General,
		CoreSystems,
		Optick,
		LinearAllocator,
		StackAllocator,
		String,
		Vector,

		MaxEnum
	};

	struct APRICOT_API HMemoryDebugger
	{
	public:
		static void DebugLog(
			bool bTotalAllocated   = true, bool bTotalFreed         = true,
			bool bTotalAllocations = true, bool bTotalDeallocations = true,
			bool bActiveAllocated  = true, bool bActiveAllocations  = true
		);

	public:
		static uint64 TotalAllocatedMemory	[(uint32)AllocTag::MaxEnum];
		static uint64 TotalFreedMemory		[(uint32)AllocTag::MaxEnum];
		static uint64 TotalAllocations		[(uint32)AllocTag::MaxEnum];
		static uint64 TotalDeallocations	[(uint32)AllocTag::MaxEnum];
	};

	/*
	* Allocates a block of memory on the global heap.
	* 
	* @param size The size (in bytes) of the requested allocation.
	* @param tag Debugging purposes only for now.
	* 
	* @returns Pointer to the allocated memory.
	*/
	inline void* Allocate(uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::TotalAllocatedMemory	[(uint32)tag] += size;
		HMemoryDebugger::TotalAllocations		[(uint32)tag] += 1;
#endif

		return ::operator new(size);
	}

	/*
	* Frees a block of memory from the global heap.
	* It should only be used as pair with 'Allocate(uint64, AllocaTag)'.
	* 
	* @param block The memory block to be freed.
	* @param size The size (in bytes) of the memory block.
	* @param tag Debugging purposes only for now.
	*/
	inline void Free(void* block, uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		if (block != nullptr)
		{
			HMemoryDebugger::TotalFreedMemory	[(uint32)tag] += size;
			HMemoryDebugger::TotalDeallocations	[(uint32)tag] += 1;
		}
#endif

		return ::operator delete(block, size);
	}

	/*
	* Allocates a block of memory on the global heap.
	* It will call the type's constructor.
	* 
	* @param typename T The type you want to allocate.
	* @param tag Debugging purposes only for now.
	*
	* @returns Pointer to the allocated memory.
	*/
	template<typename T, typename... Args>
	inline T* New(AllocTag tag, Args&&... args)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		HMemoryDebugger::TotalAllocatedMemory	[(uint32)tag] += sizeof(T);
		HMemoryDebugger::TotalAllocations		[(uint32)tag] += 1;
#endif

		return new T(std::forward<Args>(args)...);
	}

	/*
	* Creates an object at the specified address.
	* 
	* @param typename T The type you want to construct.
	* @param typename... Args The argument pack. It will be forwarded to the type's constructor.
	* @param address The memory address where the object should be constructed.
	* 
	* @returns Pointer to the constructed object. Same as parameter 'address'.
	*/
	template<typename T, typename... Args>
	inline T* PlacementNew(void* address, Args&&... args)
	{
		return new(address) T(std::forward<Args>(args)...);
	}

	/*
	* Deletes a block of memory from the global heap.
	* It calls the Deconstructor.
	* It should only be used as pair with 'New<T>(AllocTag, Args&&...)'.
	* Use the function below as much as possible, due to more accurate debugging informations.
	* 
	* @param block The memory block to be deleted.
	* @param tag Debugging purposes only for now.
	*/
	template<typename T>
	inline void Delete(T* block, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		// This data might not be real
		HMemoryDebugger::TotalFreedMemory	[(uint32)tag] += sizeof(T);
		HMemoryDebugger::TotalDeallocations	[(uint32)tag] += 1;
#endif

		delete block;
	}

	/*
	* Deletes a block of memory from the global heap.
	* It calls the Deconstructor.
	* It should only be used as pair with 'New<T>(AllocTag, Args&&...)'.
	*
	* @param block The memory block to be deleted.
	* @param size The size (in bytes) of the deleted object. Has debugging purposes for now.
	* @param tag Debugging purposes only for now.
	*/
	template<typename T>
	inline void Delete(T* block, uint64 size, AllocTag tag = AllocTag::General)
	{
#ifdef AE_ENABLE_MEMORY_TRACE
		if (block != nullptr)
		{
			HMemoryDebugger::TotalFreedMemory	[(uint32)tag] += size;
			HMemoryDebugger::TotalDeallocations	[(uint32)tag] += 1;
		}
#endif

		delete block;
	}

	/*
	* Copies the raw memory.
	*/
	inline void Copy(void* dest, const void* src, uint64 size)
	{
		memcpy(dest, src, size);
	}

	/*
	* Sets the memory to 0.
	*/
	inline void SetZero(void* block, uint64 size)
	{
		memset(block, 0, size);
	}

	/*
	* Sets the memory to the specified value.
	*/
	inline void Set(void* block, uint64 size, int value)
	{
		memset(block, value, size);
	}

} }