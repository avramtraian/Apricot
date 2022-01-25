// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"

namespace Apricot {

	enum class EAllocatorType : uint8
	{
		None = 0,

		Global,
		Linear,
		Stack,
		Dynamic,
	};

	class APRICOT_API AAllocator
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) = 0;
		virtual void Free(void* Memory, uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) = 0;
	};

	class APRICOT_API AGlobalAllocator : public AAllocator
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) override;
		virtual void Free(void* Memory, uint64 Size, EAllocTag ReservedTag = EAllocTag::Unknown) override;

		static EAllocatorType GetStaticType() { return EAllocatorType::Global; }
	};

	APRICOT_API extern AGlobalAllocator* GAllocator;

}