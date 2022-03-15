// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Hash.h"
#include "Pair.h"

#include "Apricot/Core/Memory/ApricotMemory.h"
#include "Apricot/Core/Memory/HeapAllocator.h"

#include "Iterators/HashMapIterator.h"

namespace Apricot {
	
	/**
	* C++ Core Engine Container
	*/
	template<typename KeyType, typename ValueType, typename AllocatorType = HeapAllocator>
	class THashMap
	{
	public:
		static constexpr float MaxLoadFactor = 0.5f;

		struct KeyValue
		{
			KeyType Key;
			ValueType Value;
		};

		enum EMetadataState : uint8
		{
			Occupied = 0b00000000,
			Deleted  = 0b00000001,
			Empty    = 0b00000011,
		};

		using TIterator = THashMapIterator<KeyType, ValueType>;

	public:
		THashMap()
		{
			m_Allocator = AllocatorType::GetDefault();

			m_Capacity = 101;
			auto[kvs, metadata] = AllocateMemory(m_Capacity, m_Allocator);
			m_KeyValues = kvs;
			m_Metadata = metadata;
		}

		explicit THashMap(AllocatorType* allocator, uint64 capacity = 101)
		{
			m_Allocator = allocator;
			
			m_Capacity = capacity;
			auto [kvs, metadata] = AllocateMemory(m_Capacity, m_Allocator);
			m_KeyValues = kvs;
			m_Metadata = metadata;
		}

		template<typename OtherAllocatorType>
		THashMap(const THashMap<KeyType, ValueType, OtherAllocatorType>& other)
		{
			m_Allocator = AllocatorType::GetStaticType() == OtherAllocatorType::GetStaticType() ? other.m_Allocator : AllocatorType::GetDefault();

			m_Capacity = other.m_Capacity;
			auto [kvs, metadata] = AllocateMemory(m_Capacity, m_Allocator);
			m_KeyValues = kvs;
			m_Metadata = metadata;

			for (uint64 index = 0; index < m_Capacity; index++)
			{
				if ((other.m_Metadata[index] & 0x3) == Occupied)
				{
					KeyValue& kv = m_KeyValues[index];
					const KeyValue& otherKv = other.m_KeyValues[index];

					MemConstruct<KeyType>(&kv.Key, otherKv.Key);
					MemConstruct<ValueType>(&kv.Value, otherKv.Value);

					m_Metadata[index] = other.m_Metadata[index];
				}
			}
		}

		template<typename OtherAllocatorType>
		THashMap(THashMap<KeyType, ValueType, OtherAllocatorType>&& other) noexcept
		{
			m_Capacity      = Move(other.m_Capacity);
			m_ElementsCount = Move(other.m_ElementsCount);

			if (AllocatorType::GetStaticType() == OtherAllocatorType::GetStaticType())
			{
				m_Allocator = other.m_Allocator;

				m_KeyValues = Move(other.m_KeyValues);
				m_Metadata  = Move(other.m_Metadata);
			}
			else
			{
				m_Allocator = AllocatorType::GetDefault();

				auto [kvs, metadata] = AllocateMemory(m_Capacity, m_Allocator);
				m_KeyValues = kvs;
				m_Metadata = metadata;

				for (uint64 index = 0; index < m_Capacity; index++)
				{
					if ((other.m_Metadata[index] & 0x3) == Occupied)
					{
						KeyValue& kv = m_KeyValues[index];
						KeyValue& otherKv = other.m_KeyValues[index];

						MemConstruct<KeyType>(&kv.Key, Move(otherKv.Key));
						MemConstruct<ValueType>(&kv.Value, Move(otherKv.Value));

						otherKv.Key.~KeyType();
						otherKv.Value.~ValueType();

						m_Metadata[index] = other.m_Metadata[index];
					}
				}

				FreeMemory(other.m_KeyValues, other.m_Capacity, other.m_Allocator);
			}

			other.m_KeyValues     = nullptr;
			other.m_Metadata      = nullptr;
			other.m_Capacity      = 0;
			other.m_ElementsCount = 0;
		}

		~THashMap()
		{
			if (m_KeyValues)
			{
				for (uint64 index = 0; index < m_Capacity; index++)
				{
					if ((m_Metadata[index] & 0x3ull) == Occupied)
					{
						KeyValue& kv = m_KeyValues[index];
						kv.Key.~KeyType();
						kv.Value.~ValueType();
					}
				}

				FreeMemory(m_KeyValues, m_Capacity, m_Allocator);
			}
		}

	public:
		template<typename KeyConstructType, typename ValueConstructType>
		TIterator Insert(KeyConstructType key, ValueConstructType value)
		{
			if (IsOverLoadFactor())
			{
				ResizeMap(m_Capacity + m_Capacity / 3 + 23);
			}

			uint64 keyHash = GetTypeHash<KeyType>(key);
			uint64 keyIndex = GetHash58(keyHash) % m_Capacity;

			uint8 keyHash6 = GetHash6(keyHash);

			while (true)
			{
				if ((m_Metadata[keyIndex] & 0x3) != Occupied)
				{
					m_Metadata[keyIndex] = (keyHash6 | Occupied);
					KeyValue* kv = m_KeyValues + keyIndex;
					MemConstruct<KeyType>(&kv->Key, Forward<KeyConstructType>(key));
					MemConstruct<ValueType>(&kv->Value, Forward<ValueConstructType>(value));

					m_ElementsCount++;
					return IteratorAt(keyIndex);
				}
				keyIndex = (keyIndex + 1) % m_Capacity;
			}
		}

		void Erase(const KeyType& key)
		{
			uint64 keyHash = GetTypeHash<KeyType>(key);
			uint64 keyIndex = GetHash58(keyHash) % m_Capacity;

			uint8 keyHash6 = GetHash6(keyHash);

			while (true)
			{
				if (keyHash6 == m_Metadata[keyIndex] && key == m_KeyValues[keyIndex].Key)
				{
					m_Metadata[keyIndex] = Deleted;
					KeyValue* kv = m_KeyValues + keyIndex;
					kv->Key.~KeyType();
					kv->Value.~ValueType();

					m_ElementsCount--;
					break;
				}
				if (m_Metadata[keyIndex] == Empty)
				{
					break;
				}
				keyIndex = (keyIndex + 1) % m_Capacity;
			}
		}

		TIterator Find(const KeyType& key)
		{
			uint64 keyHash = GetTypeHash<KeyType>(key);
			uint64 keyIndex = GetHash58(keyHash) % m_Capacity;

			uint8 keyHash6 = GetHash6(keyHash);

			while (true)
			{
				if (keyHash6 == m_Metadata[keyIndex] && key == m_KeyValues[keyIndex].Key)
				{
					return IteratorAt(keyIndex);
				}
				if (m_Metadata[keyIndex] == Empty)
				{
					return end();
				}
				keyIndex = (keyIndex + 1) % m_Capacity;
			}
		}

	public:
		template<typename OtherAllocatorType>
		ValueType& operator=(const THashMap<KeyType, ValueType, OtherAllocatorType>& other)
		{
			return *this;
		}

		template<typename OtherAllocatorType>
		ValueType& operator=(THashMap<KeyType, ValueType, OtherAllocatorType>&& other) noexcept
		{
			return *this;
		}

		template<typename KeyConstructType>
		ValueType& operator[](KeyConstructType key)
		{
			if (IsOverLoadFactor())
			{
				ResizeMap(m_Capacity + m_Capacity / 3 + 23);
			}

			uint64 keyHash = GetTypeHash<KeyType>(key);
			uint64 keyIndex = GetHash58(keyHash) % m_Capacity;

			uint8 keyHash6 = GetHash6(keyHash);
			uint64 bestKeyIndex = AE_UINT64_MAX;

			while (true)
			{
				if (keyHash6 == m_Metadata[keyIndex] && key == m_KeyValues[keyIndex].Key)
				{
					return m_KeyValues[keyIndex].Value;
				}
				if (m_Metadata[keyIndex] == Deleted && bestKeyIndex == AE_UINT64_MAX)
				{
					bestKeyIndex = keyIndex;
				}
				else if (m_Metadata[keyIndex] == Empty)
				{
					if (bestKeyIndex == AE_UINT64_MAX)
					{
						bestKeyIndex = keyIndex;
					}
					break;
				}
				keyIndex = (keyIndex + 1) % m_Capacity;
			}

			AE_CORE_ASSERT(bestKeyIndex != AE_UINT64_MAX);

			KeyValue& kv = m_KeyValues[bestKeyIndex];
			MemConstruct<KeyType>(&kv.Key, Forward<KeyConstructType>(key));
			MemConstruct<ValueType>(&kv.Value);

			m_Metadata[bestKeyIndex] = (keyHash6 | Occupied);
			m_ElementsCount++;
			return kv.Value;
		}

		template<typename KeyConstructType>
		const ValueType& operator[](KeyConstructType key) const
		{
			THashMap* map = (THashMap*)this;
			return (*map)[Forward<KeyConstructType>(key)];
		}

	public:
		TIterator begin()
		{
			for (uint64 index = 0; index < m_Capacity; index++)
			{
				if (GetMetadataState(m_Metadata[index]) == Occupied)
				{
					return IteratorAt(index);
				}
			}

			return end();
		}

		TIterator end()
		{
			return TIterator(m_KeyValues + m_Capacity, m_KeyValues + m_Capacity, m_Metadata + m_Capacity);
		}

		TIterator IteratorAt(uint64 index)
		{
			return TIterator(m_KeyValues + index, m_KeyValues + m_Capacity - 1, m_Metadata + index);
		}

	private:
		static TPair<KeyValue*, uint8*> AllocateMemory(uint64 capacity, AllocatorType* allocator)
		{
			uint64 memoryRequirement = (sizeof(uint8) + sizeof(KeyValue)) * capacity;
			void* memory = allocator->Alloc(memoryRequirement, EAllocatorHint::HashMap);

			KeyValue* keyValues = (KeyValue*)memory;

			uint8* metadata = (uint8*)(keyValues + capacity);
			MemSet(metadata, Empty, capacity * sizeof(uint8));

			return { keyValues, metadata };
		}

		static void FreeMemory(KeyValue* keyValues, uint64 capacity, AllocatorType* allocator)
		{
			uint64 memoryRequirement = (sizeof(uint8) + sizeof(KeyValue)) * capacity;
			allocator->Free(keyValues, memoryRequirement, EAllocatorHint::HashMap);
		}

		bool IsOverLoadFactor() const
		{
			return m_ElementsCount > (uint64)((double)m_Capacity * MaxLoadFactor);
		}

		void ResizeMap(uint64 newCapacity)
		{
			// NOTE (Avr): It can't expand the existing memory block because the hash indices will be different. And this will
			// give us a pretty hard time trying to move the data at the correct new addresses.

			THashMap newMap = THashMap(m_Allocator, newCapacity);

			for (uint64 index = 0; index < m_Capacity; index++)
			{
				if (GetMetadataState(index) == Occupied)
				{
					KeyValue& kv = m_KeyValues[index];
					newMap.Insert(Move(kv.Key), Move(kv.Value));
					kv.Key.~KeyType();
					kv.Value.~ValueType();
				}
			}

			FreeMemory(m_KeyValues, m_Capacity, m_Allocator);

			m_KeyValues     = Move(newMap.m_KeyValues);
			m_Metadata      = Move(newMap.m_Metadata);
			m_Capacity      = Move(newMap.m_Capacity);
			m_ElementsCount = Move(newMap.m_ElementsCount);

			newMap.m_KeyValues     = nullptr;
			newMap.m_Metadata      = nullptr;
			newMap.m_Capacity      = 0;
			newMap.m_ElementsCount = 0;
		}

		static uint64 GetHash58(uint64 hash) { return hash << 6; }
		static uint8 GetHash6(uint64 hash) { return (hash & 0x3Full) << 2; }

		static uint8 GetMetadataState(uint8 metadata) { return (metadata & 0x3ull); }

	private:
		AllocatorType* m_Allocator = nullptr;
		KeyValue* m_KeyValues = nullptr;
		uint8* m_Metadata = nullptr;
		uint64 m_Capacity = 0;
		uint64 m_ElementsCount = 0;
	};

}