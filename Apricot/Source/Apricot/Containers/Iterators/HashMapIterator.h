// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot {

	template<typename KeyType, typename ValueType>
	class THashMapIterator
	{
	public:
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

	public:
		THashMapIterator(void* keyValues, void* maxKeyValues, void* metadata)
			: m_KeyValues((KeyValue*)keyValues), m_MaxKeyValues((KeyValue*)maxKeyValues), m_Metadata((uint8*)metadata) {}

		bool operator==(const THashMapIterator& other) const
		{
			return m_KeyValues == other.m_KeyValues;
		}

		bool operator!=(const THashMapIterator& other) const
		{
			return !(*this == other);
		}

		THashMapIterator& operator++()
		{
			m_KeyValues++;
			m_Metadata++;
			while (m_KeyValues <= m_MaxKeyValues && ((*m_Metadata) & 0x3ull) != Occupied)
			{
				m_KeyValues++;
				m_Metadata++;
			}
			return *this;
		}

		THashMapIterator operator++(int) const
		{
			THashMapIterator temp = *this;
			++(*this);
			return temp;
		}

		KeyValue& operator*()
		{
			return *m_KeyValues;
		}

		KeyValue* operator->()
		{
			return m_KeyValues;
		}

	private:
		KeyValue* m_KeyValues = nullptr;
		KeyValue* m_MaxKeyValues = nullptr;
		uint8* m_Metadata = nullptr;
	};

}