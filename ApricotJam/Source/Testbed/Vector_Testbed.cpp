// Part of Apricot Engine. 2022-2022.

#include "ajpch.h"
#include "Testbed.h"

#include "Apricot/Containers/Vector.h"

#include "Apricot/Core/Memory/Memory.h"

#define CREATE_DESTROY_RUNS 100
#define PUSH_EMPLACE_POP_BACK_RUNS 512

namespace Apricot { namespace Testbed { 

	class CustomAllocator : public AAllocatorBase
	{
	public:
		virtual void* Allocate(uint64 Size, EAllocTag ReservedTag) override
		{
			return GHeapAllocator->Allocate(Size, ReservedTag);
		}

		virtual void Free(void* Memory, uint64 Size, EAllocTag ReservedTag) override
		{
			GHeapAllocator->Free(Memory, Size, ReservedTag);
		}

		static EAllocatorType GetStaticType() { return EAllocatorType::None; }
	};
	static CustomAllocator SAllocator;

	struct Vector_Test_Data
	{
	public:
		Vector_Test_Data(uint64)
		{
			Data = GHeapAllocator->Allocate(512, EAllocTag::Debug);
		}
		~Vector_Test_Data()
		{
			GHeapAllocator->Free(Data, 512, EAllocTag::Debug);
		}

		void* Data;
	};

	bool8 Vector_Create_Destroy()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		for (uint64 Index = 0; Index < CREATE_DESTROY_RUNS; Index++)
		{
			TVector<uint64> Vector;
		}

		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Create_Destroy_Custom_Allocators()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		for (uint64 Index = 0; Index < CREATE_DESTROY_RUNS; Index++)
		{
			TVector<uint64, CustomAllocator> Vector(&SAllocator);
		}

		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Push_Emplace_Pop_Back()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		for (uint64 Index = 0; Index < PUSH_EMPLACE_POP_BACK_RUNS / 2; Index++)
		{
				TVector<uint64> Vector;

				Vector.PushBack(0);
				Vector.PushBack(0);
				Vector.EmplaceBack(1);
				Vector.EmplaceBack(1);
				Vector.PopBack();
				Vector.PushBack(0);
				Vector.EmplaceBack(1);
				Vector.PopBack();
				Vector.PopBack();
				Vector.EmplaceBack(1);
				Vector.PopBack();
				Vector.PushBack(0);
				Vector.PopBack();
		}

		for (uint64 Index = 0; Index < PUSH_EMPLACE_POP_BACK_RUNS / 2; Index++)
		{
			TVector<Vector_Test_Data> Vector;

			Vector.PushBack(0);
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PopBack();
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.PopBack();
		}

		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Push_Emplace_Pop_Back_Custom_Allocators()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Unknown] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Unknown];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Unknown] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Unknown];

		for (uint64 Index = 0; Index < PUSH_EMPLACE_POP_BACK_RUNS / 2; Index++)
		{
			TVector<uint64, CustomAllocator> Vector(&SAllocator);

			Vector.PushBack(0);
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PopBack();
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.PopBack();
		}

		for (uint64 Index = 0; Index < PUSH_EMPLACE_POP_BACK_RUNS / 2; Index++)
		{
			TVector<Vector_Test_Data, CustomAllocator> Vector(&SAllocator);

			Vector.PushBack(0);
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PopBack();
			Vector.EmplaceBack(1);
			Vector.PopBack();
			Vector.PushBack(0);
			Vector.PopBack();
		}

		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Unknown] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Unknown];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Unknown] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Unknown];

		if (StartMemory != EndMemory)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Copy_Move()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];



		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Copy_Move_Custom_Allocators()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];



		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Clear_Tidy_Resize_Reserve()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];



		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Clear_Tidy_Resize_Reserve_Custom_Allocators()
	{
		uint64 StartMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 StartAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];



		uint64 EndMemory = AMemoryDebugger::AllocatedTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::FreedTagged[(uint16)EAllocTag::Vector];
		uint64 EndAllocs = AMemoryDebugger::AllocationsCountTagged[(uint16)EAllocTag::Vector] - AMemoryDebugger::DeallocationsCountTagged[(uint16)EAllocTag::Vector];

		if (StartMemory != EndMemory)
		{
			return false;
		}
		if (StartAllocs != EndAllocs)
		{
			return false;
		}

		return true;
	}

	void Run_Vector_Tests()
	{
		if (Vector_Create_Destroy())
		{
			AE_CORE_INFO("Vector_Create_Destroy test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Create_Destroy test failed.");
		}
		if (Vector_Create_Destroy_Custom_Allocators())
		{
			AE_CORE_INFO("Vector_Create_Destroy_Custom_Allocators test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Create_Destroy_Custom_Allocators test failed.");
		}
		if (Vector_Push_Emplace_Pop_Back())
		{
			AE_CORE_INFO("Vector_Push_Emplace_Pop_Back test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Push_Emplace_Pop_Back test failed.");
		}
		if (Vector_Push_Emplace_Pop_Back_Custom_Allocators())
		{
			AE_CORE_INFO("Vector_Push_Emplace_Pop_Back_Custom_Allocators test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Push_Emplace_Pop_Back_Custom_Allocators test failed.");
		}
		if (Vector_Copy_Move())
		{
			AE_CORE_INFO("Vector_Copy_Move test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Copy_Move test failed.");
		}
		if (Vector_Copy_Move_Custom_Allocators())
		{
			AE_CORE_INFO("Vector_Copy_Move_Custom_Allocators test passed.");
		}
		else
		{
			AE_CORE_ERROR("Vector_Copy_Move_Custom_Allocators test failed.");
		}
	}

} }