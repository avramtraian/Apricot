#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Core/Assert.h"

namespace Apricot {

	template<typename T>
	class TVector
	{
	public:
		TVector()
		{
			
		}

		TVector(uint64 capacity)
		{
			ReAllocate(capacity);
		}

		~TVector()
		{
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::Vector);
		}

	public:
		const T& operator[](uint64 index) const
		{
			AE_CORE_ASSERT(index < m_Size, "Index out of range!");
			return m_Data[index];
		}

		T& operator[](uint64 index)
		{
			AE_CORE_ASSERT(index < m_Size, "Index out of range!");
			return m_Data[index];
		}

	public:
		T* Data() const { return m_Data; }
		uint64 Capacity() const { return m_Capacity; }
		uint64 Size() const { return m_Size; }
		
		T& PushBack(const T& value)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory::PlacementNew<T>(&m_Data[m_Size], value);
			m_Size++;
			return m_Data[m_Size - 1];
		}

		T& PushBack(T&& value)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory::PlacementNew<T>(&m_Data[m_Size], std::move(value));
			m_Size++;
			return m_Data[m_Size - 1];
		}

		template<typename... Args>
		T& EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory::PlacementNew<T>(&m_Data[m_Size], std::forward<Args>(args)...);
			m_Size++;
			return m_Data[m_Size - 1];
		}

		void Resize(uint64 newSize)
		{
			if (newSize > m_Capacity)
			{
				ReAllocateCopy(newSize);
			}

			for (uint64 index = m_Size; index < newSize; index++)
			{
				m_Data[index].~T();
			}
			m_Size = newSize;
		}

		void Reserve(uint64 newCapacity)
		{
			ReAllocateCopy(newCapacity);
		}

		void Clear()
		{
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index].~T();
			}
			m_Size = 0;
		}

		void Tidy()
		{
			ReAllocateCopy(m_Size);
		}

		void TidyClear()
		{
			Clear();
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::Vector);
			m_Data = nullptr;
			m_Capacity = 0;
		}

	private:
		void ReAllocate(uint64 newCapacity)
		{
			T* newBlock = (T*)Memory::Allocate(newCapacity * sizeof(T), Memory::AllocTag::Vector);
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::Vector);

			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

		void ReAllocateCopy(uint64 newCapacity)
		{
			T* newBlock = (T*)Memory::Allocate(newCapacity * sizeof(T), Memory::AllocTag::Vector);

			uint64 newSize = m_Size;
			if (newSize > newCapacity)
			{
				newSize = newCapacity;
			}
			for (uint64 index = 0; index < newSize; index++)
			{
				Memory::PlacementNew<T>(&newBlock[index], std::move(m_Data[index]));
			}
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index].~T();
			}

			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::Vector);
			m_Data = newBlock;
			m_Capacity = newCapacity;
			m_Size = newSize;
		}

	private:
		T* m_Data = nullptr;
		uint64 m_Capacity = 0;
		uint64 m_Size = 0;
	};

}