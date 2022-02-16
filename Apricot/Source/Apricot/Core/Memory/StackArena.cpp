// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "StackArena.h"

namespace Apricot {

	uint64 AStackArena::GetTotalSize() const
	{
		return 0;
	}

	uint64 AStackArena::GetAllocatedSize() const
	{
		return 0;
	}

	uint64 AStackArena::GetFreeSize() const
	{
		return 0;
	}

	const TChar* AStackArena::GetDebugName() const
	{
		return TEXT("STACK_ARENA");
	}

	NODISCARD void* AStackArena::Alloc(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		return nullptr;
	}

	NODISCARD int32 AStackArena::TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment /*= sizeof(void*)*/)
	{
		return AE_ALLOC_SUCCESSFULLY;
	}

	NODISCARD void* AStackArena::AllocUnsafe(uint64 Size, uint64 Alignment /*= sizeof(void*)*/)
	{
		return nullptr;
	}

	void AStackArena::Free(void* Allocation, uint64 Size)
	{
		
	}

	int32 AStackArena::TryFree(void* Allocation, uint64 Size)
	{
		return AE_FREE_SUCCESSFULLY;
	}

	void AStackArena::FreeUnsafe(void* Allocation, uint64 Size)
	{
		
	}

	void AStackArena::FreeAll()
	{
		
	}

	int32 AStackArena::TryFreeAll()
	{
		return AE_FREE_SUCCESSFULLY;
	}

	void AStackArena::FreeAllUnsafe()
	{
		
	}

	void AStackArena::GarbageCollect()
	{
		
	}

	void AStackArena::Pop(uint64 Size)
	{
		
	}

	int32 AStackArena::TryPop(uint64 Size)
	{
		return AE_FREE_SUCCESSFULLY;
	}

	void AStackArena::PopUnsafe(uint64 Size)
	{
		
	}



	APRICOT_API uint64 GetStackArenaMemoryRequirementEx(const AStackArenaSpecification&)
	{
		return 0;
	}

	APRICOT_API AStackArena* CreateStackArenaEx(const AStackArenaSpecification& Specification)
	{
		return nullptr;
	}

	APRICOT_API void DestroyStackArena(AStackArena* Arena)
	{
		if (Arena)
		{
			
		}
	}

}