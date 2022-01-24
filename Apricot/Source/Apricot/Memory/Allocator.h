// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"

namespace Apricot {

	enum class AllocatorType : uint8
	{
		None = 0,

		Global,
		Linear,
		Stack,
		Dynamic,
	};

	class APRICOT_API Allocator
	{
	public:
		virtual void* Allocate(uint64 size, AllocTag reservedTag = AllocTag::Unknown) = 0;
		virtual void Free(void* memory, uint64 size, AllocTag reservedTag = AllocTag::Unknown) = 0;
	};

	class APRICOT_API GlobalAllocator : public Allocator
	{
	public:
		virtual void* Allocate(uint64 size, AllocTag reservedTag = AllocTag::Unknown) override;
		virtual void Free(void* memory, uint64 size, AllocTag reservedTag = AllocTag::Unknown) override;

		static AllocatorType GetStaticType() { return AllocatorType::Global; }
	};

	APRICOT_API extern GlobalAllocator* GAllocator;

}