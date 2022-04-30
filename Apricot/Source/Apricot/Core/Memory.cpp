#include "aepch.h"
#include "Memory.h"

#include "Containers/ApricotString.h"

#include "Platform.h"

namespace Apricot {
	
	struct AllocatorData
	{
		class AllocationsTableAllocator
		{
		public:
			static void* allocate(uint64 size)
			{
				return Allocator::AllocateRaw(size);
			}

			static void deallocate(void* memory, uint64 size)
			{
				Allocator::FreeRaw(memory);
			}
		};

		uint64 TotalAllocated = 0;
		uint64 TotalFreed = 0;

		// astl::hash_table<void*, uint64, astl::hash<void*>, AllocationsTableAllocator> AllocationsTable;
	};
	static AllocatorData s_Data;

	void* Allocator::AllocateRaw(uint64 size)
	{
		if (size == 0)
		{
			return nullptr;
		}

		return malloc(size);
	}

	void* Allocator::Allocate(uint64 size)
	{
		if (size == 0)
		{
			return nullptr;
		}

		s_Data.TotalAllocated += size;

		void* memory = AllocateRaw(size);
		// s_Data.AllocationsTable.insert(memory, size);

		return memory;
	}

	void* Allocator::Allocate(uint64 size, const char* description)
	{
		if (size == 0)
		{
			return nullptr;
		}

		void* memory = AllocateRaw(size);

		s_Data.TotalAllocated += size;
		// s_Data.AllocationsTable.insert(memory, size);

		return memory;
	}

	void* Allocator::Allocate(uint64 size, const char* file, uint32 line)
	{
		if (size == 0)
		{
			return nullptr;
		}

		void* memory = AllocateRaw(size);

		s_Data.TotalAllocated += size;
		// s_Data.AllocationsTable.insert(memory, size);

		return memory;
	}

	void Allocator::FreeRaw(void* block)
	{
		if (block == nullptr)
		{
			return;
		}

		free(block);
	}

	void Allocator::Free(void* block)
	{
		if (block == nullptr)
		{
			return;
		}

		// auto allocation = s_Data.AllocationsTable.find(block);
		// AE_CORE_ASSERT(allocation != s_Data.AllocationsTable.end()); // Allocation was not registered
		// s_Data.TotalFreed += allocation->Second;
		// TODO (Avr): Use the iterator override
		// s_Data.AllocationsTable.erase(allocation->First);

		FreeRaw(block);
	}

	void MemoryCopy(void* destination, const void* source, uint64 size)
	{
		Platform::MemoryCopy(destination, source, size);
	}

	void MemorySet(void* destination, int32 value, uint64 size)
	{
		Platform::MemorySet(destination, value, size);
	}

	void MemoryZero(void* destination, uint64 size)
	{
		Platform::MemoryZero(destination, size);
	}

	FString GetBytesName(uint64 bytesCount)
	{
		constexpr uint64 KB = 1024;
		constexpr uint64 MB = 1024 * KB;
		constexpr uint64 GB = 1024 * MB;

		static FString fmt = "{.2} {}";

		if (bytesCount >= GB)
		{
			double gbs = (double)bytesCount / (double)GB;
			// return fmt.format(gbs, "GB");
			return FString();
		}
		else if (bytesCount >= MB)
		{
			double mbs = (double)bytesCount / (double)MB;
			// return fmt.format(mbs, "MB");
			return FString();
		}
		else if (bytesCount >= KB)
		{
			double kbs = (double)bytesCount / (double)KB;
			// return fmt.format(kbs, "KB");
			return FString();
		}
		else
		{
			// return fmt.format(bytesCount, "B");
			return FString();
		}
	}

}