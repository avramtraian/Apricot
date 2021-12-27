#pragma once

#include "Base.h"
#include "Memory.h"

namespace Apricot { namespace Memory {

	class APRICOT_API DefaultAllocator
	{
	public:
		inline static void* Allocate(uint64 size, Memory::AllocTag tag = Memory::AllocTag::General)
		{
			return Memory::Allocate(size, tag);
		}

		inline static void Free(void* data, uint64 size, Memory::AllocTag tag = Memory::AllocTag::General)
		{
			Memory::Free(data, size, tag);
		}
	};

	class APRICOT_API LinearAllocator
	{
	public:
		LinearAllocator() = default;
		LinearAllocator(uint64 size, void* memoryBlock = nullptr);
		~LinearAllocator();

		LinearAllocator(const LinearAllocator&) = delete;
		LinearAllocator& operator=(const LinearAllocator&) = delete;

		void* Allocate(uint64 size);
		void FreeAll();

	public:
		static void Create(LinearAllocator* allocator, uint64 size, void* memoryBlock = nullptr);
		static void Destroy(LinearAllocator* allocator);

	private:
		uint8* m_MemoryBlock = nullptr;
		bool m_bOwnsMemory = false;
		uint64 m_TotalSize = 0;
		uint64 m_Allocated = 0;
	};

	class APRICOT_API StackAllocator
	{
	public:
		StackAllocator() = default;
		StackAllocator(uint64 size, void* memoryBlock = nullptr);
		~StackAllocator();

		StackAllocator(const StackAllocator&) = delete;
		StackAllocator& operator=(const StackAllocator&) = delete;

		void* Allocate(uint64 size);
		void Free(uint64 size);
		void Free(void* mark);
		void FreeAll();

	public:
		static void Create(StackAllocator* allocator, uint64 size, void* memoryBlock = nullptr);
		static void Destroy(StackAllocator* allocator);

	private:
		uint8* m_MemoryBlock = nullptr;
		bool m_bOwnsMemory = false;
		uint64 m_TotalSize = 0;
		uint64 m_Allocated = 0;
	};

} }