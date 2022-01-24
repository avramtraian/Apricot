// Part of Apricot Engine. 2022-2022.

#include "ajpch.h"
#include "Testbed.h"

#include "Apricot/Containers/Vector.h"

#include "Apricot/Core/Memory.h"

#define CREATE_DESTROY_RUNS 100
#define PUSH_EMPLACE_POP_BACK_RUNS 512

namespace Apricot { namespace Testbed { 

	class CustomAllocator : public Allocator
	{
	public:
		virtual void* Allocate(uint64 size, AllocTag reservedTag) override
		{
			return Memory_Alloc(size, reservedTag);
		}

		virtual void Free(void* memory, uint64 size, AllocTag reservedTag) override
		{
			Memory_Free(memory, size, reservedTag);
		}

		static AllocatorType GetStaticType() { return AllocatorType::None; }
	};
	static CustomAllocator s_Allocator;

	struct Vector_Test_Data
	{
	public:
		Vector_Test_Data(uint64)
		{
			Data = Memory_Alloc(512, AllocTag::Unknown);
		}
		~Vector_Test_Data()
		{
			Memory_Free(Data, 512, AllocTag::Unknown);
		}

		void* Data;
	};

	bool8 Vector_Create_Destroy()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		for (uint64 index = 0; index < CREATE_DESTROY_RUNS; index++)
		{
			TVector<uint64> vector;
		}

		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Create_Destroy_Custom_Allocators()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		for (uint64 index = 0; index < CREATE_DESTROY_RUNS; index++)
		{
			TVector<uint64, CustomAllocator> vector(&s_Allocator);
		}

		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Push_Emplace_Pop_Back()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		for (uint64 index = 0; index < PUSH_EMPLACE_POP_BACK_RUNS / 2; index++)
		{
				TVector<uint64> vector;

				vector.PushBack(0);
				vector.PushBack(0);
				vector.EmplaceBack(1);
				vector.EmplaceBack(1);
				vector.PopBack();
				vector.PushBack(0);
				vector.EmplaceBack(1);
				vector.PopBack();
				vector.PopBack();
				vector.EmplaceBack(1);
				vector.PopBack();
				vector.PushBack(0);
				vector.PopBack();
		}

		for (uint64 index = 0; index < PUSH_EMPLACE_POP_BACK_RUNS / 2; index++)
		{
			TVector<Vector_Test_Data> vector;

			vector.PushBack(0);
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PopBack();
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.PopBack();
		}

		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Push_Emplace_Pop_Back_Custom_Allocators()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Unknown] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Unknown];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Unknown] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Unknown];

		for (uint64 index = 0; index < PUSH_EMPLACE_POP_BACK_RUNS / 2; index++)
		{
			TVector<uint64, CustomAllocator> vector(&s_Allocator);

			vector.PushBack(0);
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PopBack();
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.PopBack();
		}

		for (uint64 index = 0; index < PUSH_EMPLACE_POP_BACK_RUNS / 2; index++)
		{
			TVector<Vector_Test_Data, CustomAllocator> vector(&s_Allocator);

			vector.PushBack(0);
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PopBack();
			vector.EmplaceBack(1);
			vector.PopBack();
			vector.PushBack(0);
			vector.PopBack();
		}

		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Unknown] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Unknown];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Unknown] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Unknown];

		if (startMemory != endMemory)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Copy_Move()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];



		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Copy_Move_Custom_Allocators()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];



		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Clear_Tidy_Resize_Reserve()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];



		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
		{
			return false;
		}

		return true;
	}

	bool8 Vector_Clear_Tidy_Resize_Reserve_Custom_Allocators()
	{
		uint64 startMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 startAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];



		uint64 endMemory = MemoryDebugger::AllocatedTagged[(uint16)AllocTag::Vector] - MemoryDebugger::FreedTagged[(uint16)AllocTag::Vector];
		uint64 endAllocs = MemoryDebugger::AllocationsCountTagged[(uint16)AllocTag::Vector] - MemoryDebugger::DeallocationsCountTagged[(uint16)AllocTag::Vector];

		if (startMemory != endMemory)
		{
			return false;
		}
		if (startAllocs != endAllocs)
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