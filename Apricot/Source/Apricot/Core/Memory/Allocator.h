// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Memory.h"
#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

namespace Apricot {

	enum class EAllocatorType : uint8
	{
		None = 0,

		Global,
		Heap,
		Linear,
		Stack,
		Dynamic,
	};

	class APRICOT_API AAllocatorBase
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) = 0;
		virtual void Free(void* Memory, uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) = 0;
	};

	class APRICOT_API AGlobalAllocator : public AAllocatorBase
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) override;
		virtual void Free(void* Memory, uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) override;

		static EAllocatorType GetStaticType() { return EAllocatorType::Global; }
	};

	APRICOT_API extern AGlobalAllocator* GAllocator;

}