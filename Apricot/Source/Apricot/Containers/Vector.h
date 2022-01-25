// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"

#include "Apricot/Memory/Allocator.h"

namespace Apricot {

	/*
	* Apricot Engine Vector implementation.
	* 
	* @tparam T The type that the vector stores.
	* @tparam Alloc The allocator class used for managing the internal memory. Default value = it allocates directly from the global heap.
	*/
	template<typename T, typename Alloc = GlobalAllocator>
	class TVector
	{
	public:
		TVector(Alloc* allocator = nullptr)
		{
			m_Data = nullptr;
			m_Capacity = 0;
			m_Size = 0;

			if (allocator == nullptr)
			{
				if (Alloc::GetStaticType() == AllocatorType::Global)
				{
					m_Allocator = (Alloc*)GAllocator;
				}
				else
				{
					AE_CHECK(false);
					m_Allocator = nullptr;
				}
			}
			else
			{
				m_Allocator = allocator;
			}
		}

		TVector(uint64 capacity, Alloc* allocator = nullptr)
		{
			if (allocator == nullptr)
			{
				if (Alloc::GetStaticType() == AllocatorType::Global)
				{
					m_Allocator = (Alloc*)GAllocator;
				}
				else
				{
					AE_CHECK(false);
					m_Allocator = nullptr;
				}
			}
			else
			{
				m_Allocator = allocator;
			}

			m_Data = nullptr;
			m_Capacity = 0;
			ReAllocate(capacity);
			m_Size = 0;
		}

		TVector(const TVector<T, Alloc>& other)
		{
			m_Allocator = other.m_Allocator;
			ReAllocate(other.m_Size);

			for (uint64 index = 0; index < other.m_Size; index++)
			{
				Memory_Placement<T>(m_Data + index, other.m_Data[index]);
			}
		}

		TVector(TVector<T, Alloc>&& other) noexcept
		{
			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			m_Allocator = other.m_Allocator;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
		}

		~TVector()
		{
			ClearNoShrink();
			DeleteMemory();
		}

	public:
		T* Data() const { return m_Data; }
		uint64 Capacity() const { return m_Capacity; }
		uint64 Size() const { return m_Size; }

		bool8 IsEmpty() const { return (m_Size == 0); }

		/*
		* 
		*/
		T& PushBack(const T& element)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory_Placement<T>(m_Data + m_Size, element);

			T& value = m_Data[m_Size];
			m_Size++;
			return value;
		}

		/*
		*
		*/
		T& PushBack(T&& element)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory_Placement<T>(m_Data + m_Size, std::move(element));

			T& value = m_Data[m_Size];
			m_Size++;
			return value;
		}

		/*
		*
		*/
		template<typename... Args>
		T& EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocateCopy(m_Capacity + m_Capacity / 2 + 1);
			}

			Memory_Placement<T>(m_Data + m_Size, std::forward<Args>(args)...);

			T& value = m_Data[m_Size];
			m_Size++;
			return value;
		}

		/*
		*
		*/
		void PopBack()
		{
			AE_DEBUG_CHECK(m_Size > 0);
			m_Size--;
			m_Data[m_Size].~T();
		}

		// TODO: Implement Insert, Emplace, Erase

		/*
		*
		*/
		void Resize(uint64 newSize)
		{
			for (uint64 index = newSize; index < m_Size; index++)
			{
				m_Data[index].~T();
			}
			m_Size = newSize;

			if (newSize > m_Capacity)
			{
				ReAllocateCopy(newSize);
			}
		}

		/*
		*
		*/
		void Reserve(uint64 newCapacity)
		{
			if (newCapacity < m_Size)
			{
				for (uint64 index = newCapacity; index < m_Size; index++)
				{
					m_Data[index].~T();
				}
				m_Size = newCapacity;
			}

			if (newCapacity > m_Capacity)
			{
				ReAllocateCopy(newCapacity);
			}
		}

		/*
		*
		*/
		void Clear()
		{
			ClearNoShrink();
			DeleteMemory();
			m_Data = nullptr;
			m_Capacity = 0;
		}

		/*
		*
		*/
		void Shrink()
		{
			ReAllocateCopy(m_Size);
		}

		/*
		*
		*/
		void ClearNoShrink()
		{
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index].~T();
			}
			m_Size = 0;
		}

		/*
		*
		*/
		T& At(uint64 index)
		{
			AE_DEBUG_CHECK(index < m_Size);
			return m_Data[index];
		}

		/*
		*
		*/
		const T& At(uint64 index) const
		{
			AE_DEBUG_CHECK(index < m_Size);
			return m_Data[index];
		}

		/*
		*
		*/
		T& Front()
		{
			AE_DEBUG_CHECK(m_Size > 0);
			return m_Data[0];
		}

		/*
		*
		*/
		const T& Front() const
		{
			AE_DEBUG_CHECK(m_Size > 0);
			return m_Data[0];
		}

		/*
		*
		*/
		T& Back()
		{
			AE_DEBUG_CHECK(m_Size > 0);
			return m_Data[m_Size - 1];
		}

		/*
		*
		*/
		const T& Back() const
		{
			AE_DEBUG_CHECK(m_Size > 0);
			return m_Data[m_Size - 1];
		}

		/*
		*
		*/
		Alloc* GetAllocator() const
		{
			return m_Allocator;
		}

		/*
		*
		*/
		void SetAllocator(Alloc* newAllocator)
		{
			AE_CHECK(newAllocator != nullptr);

			if ((void*)m_Allocator == (void*)newAllocator)
			{
				return;
			}

			T* newBlock = (T*)newAllocator->Allocate(m_Capacity * sizeof(T), AllocTag::Vector);

			for (uint64 index = 0; index < m_Size; index++)
			{
				Memory_Placement<T>(newBlock + index, std::move(m_Data[index]));
				m_Data[index].~T();
			}

			m_Allocator->Free(m_Data, m_Capacity * sizeof(T), AllocTag::Vector);

			m_Data = newBlock;
			m_Allocator = newAllocator;
		}

		/*
		*
		*/
		template<typename A>
		void Swap(TVector<T, A>&& vector)
		{
			if ((void*)m_Allocator == (void*)vector.m_Allocator)
			{
				T* tempData = m_Data;
				uint64 tempCapacity = m_Capacity;
				uint64 tempSize = m_Size;

				m_Data = vector.m_Data;
				m_Capacity = vector.m_Capacity;
				m_Size = vector.m_Size;

				vector.m_Data = tempData;
				vector.m_Capacity = tempCapacity;
				vector.m_Size = tempSize;
			}
			else
			{
				// NOTE: This could be implemented so it determines at runtime if any new memory should be allocated, or to reuse the existing one.
				//           The reason why we don't to that is because it can get very difficult to read, understand, or modify the code.
				//           Also, this function is rarely called anyway.
				// TODO: Maybe implement? If there are any performance problems caused by this.

				T* newBlock = (T*)m_Allocator->Allocate(vector.m_Size * sizeof(T), AllocTag::Vector);
				for (uint64 index = 0; index < vector.m_Size; index++)
				{
					Memory_Placement<T>(newBlock + index, std::move(vector.m_Data[index]));
					vector.m_Data[index].~T();
				}

				T* vector_newBlock = (T*)vector.m_Allocator->Allocate(m_Size * sizeof(T), AllocTag::Vector);
				for (uint64 index = 0; index < m_Size; index++)
				{
					Memory_Placement<T>(vector_newBlock + index, std::move(m_Data[index]));
					m_Data[index].~T();
				}

				m_Allocator->Free(m_Data, m_Capacity * sizeof(T), AllocTag::Vector);
				vector.m_Allocator->Free(vector.m_Data, vector.m_Capacity * sizeof(T), AllocTag::Vector);

				m_Data = newBlock;
				vector.m_Data = vector_newBlock;

				m_Capacity = vector.m_Size;
				vector.m_Capacity = m_Size;

				m_Size = m_Capacity;
				vector.m_Size = vector.m_Capacity;
			}
		}

	public:
		T& operator[](uint64 index)
		{
			AE_DEBUG_CHECK(index < m_Size);
			return m_Data[index];
		}

		const T& operator[](uint64 index) const
		{
			AE_DEBUG_CHECK(index < m_Size);
			return m_Data[index];
		}

		template<typename A>
		TVector<T, Alloc>& operator=(const TVector<T, A>& other)
		{
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index].~T();
			}

			if ((void*)m_Allocator != (void*)other.m_Allocator)
			{
				DeleteMemory();
				m_Allocator = other.m_Allocator;
				ReAllocate(other.m_Size);
			}
			else if (other.m_Size > m_Capacity)
			{
				ReAllocate(other.m_Size);
			}

			for (uint64 index = 0; index < other.m_Size; index++)
			{
				Memory_Placement<T>(m_Data + index, other.m_Data[index]);
			}

			m_Size = other.m_Size;

			return *this;
		}

		TVector<T, Alloc>& operator=(TVector<T, Alloc>&& other) noexcept
		{
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index].~T();
			}
			DeleteMemory();

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			m_Allocator = other.m_Allocator;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;

			return *this;
		}

	private:
		/*
		*
		*/
		void ReAllocate(uint64 newCapacity)
		{
			DeleteMemory();
			m_Data = (T*)m_Allocator->Allocate(newCapacity * sizeof(T), AllocTag::Vector);
			m_Capacity = newCapacity;
		}

		/*
		*
		*/
		void ReAllocateCopy(uint64 newCapacity)
		{
			T* newBlock = (T*)m_Allocator->Allocate(newCapacity * sizeof(T), AllocTag::Vector);

			for (uint64 index = 0; index < m_Size; index++)
			{
				Memory_Placement<T>(newBlock + index, std::move(m_Data[index]));
				m_Data[index].~T();
			}

			DeleteMemory();
			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

		/*
		*
		*/
		void DeleteMemory()
		{
			m_Allocator->Free(m_Data, m_Capacity * sizeof(T), AllocTag::Vector);
		}

	private:
		/*
		* 
		*/
		T* m_Data;

		/*
		* 
		*/
		uint64 m_Capacity;

		/*
		* 
		*/
		uint64 m_Size;

		/*
		* 
		*/
		Alloc* m_Allocator;
	};

}