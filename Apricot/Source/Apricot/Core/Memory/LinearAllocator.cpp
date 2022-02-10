#include "aepch.h"
#include "LinearAllocator.h"

namespace Apricot {

	ALinearAllocator::ALinearAllocator()
		: m_Memory(nullptr), m_TotalSize(0), m_Allocated(0), m_OwnsMemory(false)
	{
	}

	ALinearAllocator::ALinearAllocator(ALinearAllocator&& Other) noexcept
		: m_Memory(Other.m_Memory), m_TotalSize(Other.m_TotalSize), m_Allocated(Other.m_Allocated), m_OwnsMemory(Other.m_OwnsMemory)
	{
		Other.m_Memory = nullptr;
		Other.m_TotalSize = 0;
		Other.m_Allocated = 0;
		Other.m_OwnsMemory = false;
	}

	ALinearAllocator& ALinearAllocator::operator=(ALinearAllocator&& Other) noexcept
	{
		m_Memory = Other.m_Memory;
		m_TotalSize = Other.m_TotalSize;
		m_Allocated = Other.m_Allocated;
		m_OwnsMemory = Other.m_OwnsMemory;

		Other.m_Memory = nullptr;
		Other.m_TotalSize = 0;
		Other.m_Allocated = 0;
		Other.m_OwnsMemory = false;

		return *this;
	}

	ALinearAllocator::~ALinearAllocator()
	{
		if (m_Memory && m_OwnsMemory)
		{
			AE_CORE_ERROR(TEXT("Linear Allocator not destroyed! Always call ALinearAllocator::Destroy!"));
			AE_DEBUG_CHECK(false);
		}
	}

	void ALinearAllocator::Create(ALinearAllocator* Allocator, uint64 TotalSize, void* Memory /*= nullptr*/)
	{
		if (Allocator)
		{
			if (Allocator->m_Memory)
			{
				AE_CORE_ERROR(TEXT("Attempting to create an allocator which is already initialized!"));
				return;
			}
			if (TotalSize != 0)
			{
				AE_CORE_WARN(TEXT("Attempting to create an ALinearAllocator with 0 total size!"));
				return;
			}
			
			if (Memory)
			{
				Allocator->m_Memory = Memory;
				Allocator->m_TotalSize = TotalSize;
				Allocator->m_Allocated = 0;
				Allocator->m_OwnsMemory = false;
			}
			else
			{
				Allocator->m_Memory = Memory_Alloc(TotalSize, EAllocSubregion::LinearAllocator);
				Allocator->m_TotalSize = TotalSize;
				Allocator->m_Allocated = 0;
				Allocator->m_OwnsMemory = true;
			}
		}
		else
		{
			AE_CORE_WARN(TEXT("ALinearAllocator::Create -> Invalid Allocator parameter."));
		}
	}

	void ALinearAllocator::Destroy(ALinearAllocator* Allocator)
	{
		if (Allocator && Allocator->m_Memory)
		{
			if (Allocator->m_TotalSize == 0)
			{
				AE_CORE_WARN(TEXT("Attempting to destroy an ALinearAllocator with valid memory pointer, but 0 total size. Corruption?"));
				return;
			}

			if (Allocator->m_OwnsMemory)
			{
				Memory_Free(Allocator->m_Memory, Allocator->m_TotalSize, EAllocSubregion::LinearAllocator);
			}
			Allocator->m_Memory = nullptr;
			Allocator->m_TotalSize = 0;
			Allocator->m_Allocated = 0;
			Allocator->m_OwnsMemory = false;
		}
		else
		{
			AE_CORE_WARN(TEXT("ALinearAllocator::Destroy -> Invalid or already destroyed Allocator parameter."));
		}
	}

	void* ALinearAllocator::Allocate(uint64 Size, EAllocTag Tag /*= EAllocTag::Unknown*/)
	{
		AE_DEBUG_CHECK(Size > 0); /* Invalid Size */

		AE_DEBUG_CHECK(m_Memory != nullptr); /* Invalid Allocator */

		AE_DEBUG_CHECK(m_TotalSize >= m_Allocated + Size); /* Not enough memory */

		void* Address = ((uint8*)m_Memory) + m_Allocated;
		m_Allocated += Size;
		return Address;
	}

	void ALinearAllocator::Free(void* Memory, uint64 Size, EAllocTag Tag /*= EAllocTag::Unknown*/)
	{
		AE_CHECK_NO_ENTRY();
	}

	void ALinearAllocator::FreeAll()
	{
		m_Allocated = 0;
	}

}