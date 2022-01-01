// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Allocators.h"

namespace Apricot { namespace Memory {

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////--- LINEAR ALLOCATOR ---//////////////////////////
	///////////////////////////////////////////////////////////////////////////

	LinearAllocator::LinearAllocator(uint64 size, void* memoryBlock /*= nullptr*/)
	{
		LinearAllocator::Create(this, size, memoryBlock);
	}

	LinearAllocator::~LinearAllocator()
	{
		LinearAllocator::Destroy(this);
	}

	void* LinearAllocator::Allocate(uint64 size)
	{
		AE_CORE_VERIFY_RETURN(m_Allocated + size <= m_TotalSize, nullptr, "LinearAllocator memory overflow!");

		void* memory = m_MemoryBlock + m_Allocated;
		m_Allocated += size;
		return memory;
	}

	void LinearAllocator::FreeAll()
	{
		m_Allocated = 0;
	}

	void LinearAllocator::Create(LinearAllocator* allocator, uint64 size, void* memoryBlock /*= nullptr*/)
	{
		if (memoryBlock)
		{
			allocator->m_MemoryBlock = (uint8*)memoryBlock;
			allocator->m_bOwnsMemory = false;
		}
		else
		{
			allocator->m_MemoryBlock = (uint8*)Memory::Allocate(size, AllocTag::LinearAllocator);
			allocator->m_bOwnsMemory = true;
		}
		allocator->m_TotalSize = size;
		allocator->m_Allocated = 0;
	}

	void LinearAllocator::Destroy(LinearAllocator* allocator)
	{
		if (allocator->m_bOwnsMemory && allocator->m_MemoryBlock)
		{
			Memory::Free(allocator->m_MemoryBlock, allocator->m_TotalSize, AllocTag::LinearAllocator);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////--- STACK ALLOCATOR ---///////////////////////////
	///////////////////////////////////////////////////////////////////////////

	StackAllocator::StackAllocator(uint64 size, void* memoryBlock /*= nullptr*/)
	{
		StackAllocator::Create(this, size, memoryBlock);
	}

	StackAllocator::~StackAllocator()
	{
		StackAllocator::Destroy(this);
	}

	void* StackAllocator::Allocate(uint64 size)
	{
		AE_CORE_VERIFY_RETURN(m_Allocated + size <= m_TotalSize, nullptr, "LinearAllocator memory overflow!");

		void* memory = m_MemoryBlock + m_Allocated;
		m_Allocated += size;
		return memory;
	}

	void StackAllocator::Free(uint64 size)
	{
		m_Allocated -= size;
	}

	void StackAllocator::Free(void* mark)
	{
		AE_CORE_VERIFY_RETURN(m_MemoryBlock <= mark && mark <= m_MemoryBlock + m_Allocated, , "Mark out of range for this allocator!");

		m_Allocated = (uint8*)mark - m_MemoryBlock;
	}

	void StackAllocator::FreeAll()
	{
		m_Allocated = 0;
	}

	void StackAllocator::Create(StackAllocator* allocator, uint64 size, void* memoryBlock /*= nullptr*/)
	{
		if (memoryBlock)
		{
			allocator->m_MemoryBlock = (uint8*)memoryBlock;
			allocator->m_bOwnsMemory = false;
		}
		else
		{
			allocator->m_MemoryBlock = (uint8*)Memory::Allocate(size, AllocTag::StackAllocator);
			allocator->m_bOwnsMemory = true;
		}
		allocator->m_TotalSize = size;
		allocator->m_Allocated = 0;
	}

	void StackAllocator::Destroy(StackAllocator* allocator)
	{
		if (allocator->m_bOwnsMemory && allocator->m_MemoryBlock)
		{
			Memory::Free(allocator->m_MemoryBlock, allocator->m_TotalSize, AllocTag::StackAllocator);
		}
	}

} }