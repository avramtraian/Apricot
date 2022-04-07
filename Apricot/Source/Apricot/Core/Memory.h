#pragma once

#include "Base.h"

#define MemCpy  ::Apricot::MemoryCopy
#define MemSet  ::Apricot::MemorySet
#define MemZero ::Apricot::MemoryZero

constexpr uint64 Kilo2Bytes = 1024;
constexpr uint64 Mega2Bytes = 1024 * 1024;
constexpr uint64 Giga2Bytes = 1024 * 1024 * 1024;

namespace Apricot {

	class APRICOT_API Allocator
	{
	public:
		static void* AllocateRaw(uint64 size);
		static void* Allocate(uint64 size);
		static void* Allocate(uint64 size, const char* description);
		static void* Allocate(uint64 size, const char* file, uint32 line);
		static void FreeRaw(void* block);
		static void Free(void* block);
	};

	APRICOT_API void MemoryCopy(void* destination, const void* source, uint64 size);
	APRICOT_API void MemorySet(void* destination, int32 value, uint64 size);
	APRICOT_API void MemoryZero(void* destination, uint64 size);

	APRICOT_API astl::string GetBytesName(uint64 bytesCount);

}

inline void* operator new(size_t size)
{
	return Apricot::Allocator::Allocate((uint64)size);
}

inline void* operator new(size_t size, const char* file, int line)
{
	return Apricot::Allocator::Allocate((uint64)size, file, line);
}

inline void operator delete(void* block)
{
	Apricot::Allocator::Free(block);
}

inline void operator delete(void* block, size_t size)
{
	Apricot::Allocator::Free(block);
}

inline void operator delete(void* block, const char* file, int line)
{
	Apricot::Allocator::Free(block);
}

inline void operator delete(void* block, size_t size, const char* file, int line)
{
	Apricot::Allocator::Free(block);
}

inline void operator delete[](void* block)
{
	Apricot::Allocator::Free(block);
}

inline void operator delete[](void* block, size_t size)
{
	Apricot::Allocator::Free(block);
}

#define anew new(__FILE__, __LINE__)
#define adelete delete