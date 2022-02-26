// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory/Memory.h"

#include "Apricot/Core/Memory/HeapAllocator.h"

#include "Iterators/VectorIterator.h"

#include <initializer_list>

namespace Apricot {

	/*
	* Apricot Engine Vector implementation.
	* 
	* @tparam T The type that the vector stores.
	* @tparam Alloc The allocator class used for managing the internal memory. Default value = it allocates directly from the global heap.
	*/
	template<typename T, typename Alloc = AHeapAllocator>
	class TVector
	{
	/* Typedefs */
	public:
		using ValueType = T;

		using TIterator             = TVectorIterator<T>;
		using TConstIterator        = TVectorIterator<const T>;
		using TReverseIterator      = TVectorIterator<T>;
		using TReverseConstIterator = TVectorIterator<const T>;

	public:
		TVector(Alloc* Allocator = nullptr)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			if (Allocator == nullptr)
			{
				if (Alloc::GetStaticType() == EAllocatorType::Heap)
				{
					m_Allocator = (Alloc*)GHeapAllocator;
				}
				else
				{
					AE_CHECK(false);
					m_Allocator = nullptr;
				}
			}
			else
			{
				m_Allocator = Allocator;
			}
		}

		TVector(uint64 Capacity, Alloc* Allocator = nullptr)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			if (Allocator == nullptr)
			{
				if (Alloc::GetStaticType() == EAllocatorType::Global)
				{
					m_Allocator = (Alloc*)GHeapAllocator;
				}
				else
				{
					AE_CHECK(false);
					m_Allocator = nullptr;
				}
			}
			else
			{
				m_Allocator = Allocator;
			}

			m_Data = nullptr;
			m_Capacity = 0;
			ReAllocate(Capacity);
			m_Size = 0;
		}

		TVector(const TVector<T, Alloc>& Other)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			m_Allocator = Other.m_Allocator;
			ReAllocate(Other.m_Size);
			m_Size = Other.m_Size;

			for (uint64 Index = 0; Index < Other.m_Size; Index++)
			{
				Memory_Placement<T>(m_Data + Index, Other.m_Data[Index]);
			}
		}

		TVector(TVector<T, Alloc>&& other) noexcept
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			m_Allocator = other.m_Allocator;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
		}

		TVector(std::initializer_list<T> IntializerList)
			: m_Data(nullptr), m_Allocator(GHeapAllocator), m_Capacity(0), m_Size(0)
		{
			ReAllocate(IntializerList.size());

			for (auto Element : IntializerList)
			{
				MemConstruct<T>(m_Data + m_Size, Element);
				m_Size++;
			}
		}

		~TVector()
		{
			ClearNoShrink();
			DeleteMemory();
		}

	public:
		FORCEINLINE T* Data() const { return m_Data; }
		FORCEINLINE uint64 Capacity() const { return m_Capacity; }
		FORCEINLINE uint64 Size() const { return m_Size; }

		FORCEINLINE bool8 IsEmpty() const { return (m_Size == 0); }

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

			T& Value = m_Data[m_Size];
			m_Size++;
			return Value;
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

			T& Value = m_Data[m_Size];
			m_Size++;
			return Value;
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

			T& Value = m_Data[m_Size];
			m_Size++;
			return Value;
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

		// TODO: Implement Insert, Emplace

		void Erase(TIterator First, TIterator Last)
		{
			AE_DEBUG_CHECK(First < Last);

			Erase(First, Last - First);
		}

		void Erase(TIterator Element, uint64 Count = 1)
		{
			AE_DEBUG_CHECK(m_Data <= Element);

			Erase(Element - m_Data, Count);
		}

		void Erase(uint64 ErasureIndex, uint64 Count = 1)
		{
			AE_DEBUG_CHECK(ErasureIndex + Count <= m_Size);

			for (uint64 Index = ErasureIndex; Index < m_Size - Count; Index++)
			{
				m_Data[Index].~T();
				MemConstruct<T>(m_Data + Index, Move(m_Data[Index + Count]));
			}
			m_Size -= Count;
		}

		/*
		*
		*/
		void SetSize(uint64 NewSize)
		{
			for (uint64 Index = NewSize; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}
			m_Size = NewSize;

			if (NewSize > m_Capacity)
			{
				ReAllocateCopy(NewSize);
			}
		}

		/*
		*
		*/
		void SetCapacity(uint64 NewCapacity)
		{
			if (NewCapacity < m_Size)
			{
				for (uint64 Index = NewCapacity; Index < m_Size; Index++)
				{
					m_Data[Index].~T();
				}
				m_Size = NewCapacity;
			}

			if (NewCapacity > m_Capacity)
			{
				ReAllocateCopy(NewCapacity);
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
			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}
			m_Size = 0;
		}

		/*
		*
		*/
		T& At(uint64 Index)
		{
			AE_DEBUG_CHECK(Index < m_Size);
			return m_Data[Index];
		}

		/*
		*
		*/
		const T& At(uint64 Index) const
		{
			AE_DEBUG_CHECK(Index < m_Size);
			return m_Data[Index];
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
		void SetAllocator(Alloc* NewAllocator)
		{
			AE_CHECK(NewAllocator != nullptr);

			if ((void*)m_Allocator == (void*)NewAllocator)
			{
				return;
			}

			T* NewBlock = (T*)NewAllocator->Allocate(m_Capacity * sizeof(T), EAllocTag::Vector);

			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				Memory_Placement<T>(NewBlock + Index, std::move(m_Data[Index]));
				m_Data[Index].~T();
			}

			m_Allocator->Free(m_Data, m_Capacity * sizeof(T), EAllocTag::Vector);

			m_Data = NewBlock;
			m_Allocator = NewAllocator;
		}

		/*
		*
		*/
		template<typename A>
		void Swap(TVector<T, A>&& Vector)
		{
			if ((void*)m_Allocator == (void*)Vector.m_Allocator)
			{
				T* TempData = m_Data;
				uint64 TempCapacity = m_Capacity;
				uint64 TempSize = m_Size;

				m_Data = Vector.m_Data;
				m_Capacity = Vector.m_Capacity;
				m_Size = Vector.m_Size;

				Vector.m_Data = TempData;
				Vector.m_Capacity = TempCapacity;
				Vector.m_Size = TempSize;
			}
			else
			{
				// NOTE: This could be implemented so it determines at runtime if any new memory should be allocated, or to reuse the existing one.
				//           The reason why we don't to that is because it can get very difficult to read, understand, or modify the code.
				//           Also, this function is rarely called anyway.
				// TODO: Maybe implement? If there are any performance problems caused by this.

				T* NewBlock = (T*)m_Allocator->Allocate(Vector.m_Size * sizeof(T), EAllocTag::Vector);
				for (uint64 Index = 0; Index < Vector.m_Size; Index++)
				{
					Memory_Placement<T>(NewBlock + Index, std::move(Vector.m_Data[Index]));
					Vector.m_Data[Index].~T();
				}

				T* Vector_NewBlock = (T*)Vector.m_Allocator->Allocate(m_Size * sizeof(T), EAllocTag::Vector);
				for (uint64 Index = 0; Index < m_Size; Index++)
				{
					Memory_Placement<T>(Vector_NewBlock + Index, std::move(m_Data[Index]));
					m_Data[Index].~T();
				}

				m_Allocator->Free(m_Data, m_Capacity * sizeof(T), EAllocTag::Vector);
				Vector.m_Allocator->Free(Vector.m_Data, Vector.m_Capacity * sizeof(T), EAllocTag::Vector);

				m_Data = NewBlock;
				Vector.m_Data = Vector_NewBlock;

				m_Capacity = Vector.m_Size;
				Vector.m_Capacity = m_Size;

				m_Size = m_Capacity;
				Vector.m_Size = Vector.m_Capacity;
			}
		}

	public:
		T& operator[](uint64 Index)
		{
			AE_DEBUG_CHECK(Index < m_Size);
			return m_Data[Index];
		}

		const T& operator[](uint64 Index) const
		{
			AE_DEBUG_CHECK(Index < m_Size);
			return m_Data[Index];
		}

		template<typename A>
		TVector<T, Alloc>& operator=(const TVector<T, A>& Other)
		{
			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}

			if ((void*)m_Allocator != (void*)Other.m_Allocator)
			{
				DeleteMemory();
				m_Allocator = Other.m_Allocator;
				ReAllocate(Other.m_Size);
			}
			else if (Other.m_Size > m_Capacity)
			{
				ReAllocate(Other.m_Size);
			}

			for (uint64 Index = 0; Index < Other.m_Size; Index++)
			{
				Memory_Placement<T>(m_Data + Index, Other.m_Data[Index]);
			}

			m_Size = Other.m_Size;

			return *this;
		}

		TVector<T, Alloc>& operator=(TVector<T, Alloc>&& Other) noexcept
		{
			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}
			DeleteMemory();

			m_Data = Other.m_Data;
			m_Capacity = Other.m_Capacity;
			m_Size = Other.m_Size;
			m_Allocator = Other.m_Allocator;

			Other.m_Data = nullptr;
			Other.m_Capacity = 0;
			Other.m_Size = 0;

			return *this;
		}

	/* Iterators */
	public:
		TIterator begin()
		{
			return TIterator(m_Data);
		}

		TIterator end()
		{
			return TIterator(m_Data + m_Size);
		}

		TConstIterator begin() const
		{
			return TConstIterator(m_Data);
		}

		TConstIterator end() const
		{
			return TConstIterator(m_Data + m_Size);
		}

		TReverseIterator rbegin()
		{
			return TReverseIterator(m_Data + m_Size - 1);
		}

		TReverseIterator rend()
		{
			return TReverseIterator(m_Data - 1);
		}

		TReverseConstIterator rbegin() const
		{
			return TReverseConstIterator(m_Data + m_Size - 1);
		}

		TReverseConstIterator rend() const
		{
			return TReverseConstIterator(m_Data - 1);
		}

	// Raw Memory Accesses
	public:
		void Raw_SetSize(uint64 NewSize)
		{
			m_Size = NewSize;
		}

		void Raw_SetCapacity(uint64 NewCapacity)
		{
			m_Capacity = NewCapacity;
		}

	private:
		/*
		*
		*/
		void ReAllocate(uint64 NewCapacity)
		{
			DeleteMemory();
			m_Data = (T*)m_Allocator->Allocate(NewCapacity * sizeof(T), EAllocTag::Vector);
			m_Capacity = NewCapacity;
		}

		/*
		*
		*/
		void ReAllocateCopy(uint64 NewCapacity)
		{
			T* NewBlock = (T*)m_Allocator->Allocate(NewCapacity * sizeof(T), EAllocTag::Vector);

			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				Memory_Placement<T>(NewBlock + Index, std::move(m_Data[Index]));
				m_Data[Index].~T();
			}

			DeleteMemory();
			m_Data = NewBlock;
			m_Capacity = NewCapacity;
		}

		/*
		*
		*/
		void DeleteMemory()
		{
			m_Allocator->Free(m_Data, m_Capacity * sizeof(T), EAllocTag::Vector);
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