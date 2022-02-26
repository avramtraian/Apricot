// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory/ApricotMemory.h"

#include "Iterators/VectorIterator.h"

#include <initializer_list>

namespace Apricot {

	/*
	* Apricot Engine Vector implementation.
	* 
	* @tparam T The type that the vector stores.
	* @tparam Alloc The allocator class used for managing the internal memory. Default value = it allocates directly from the global heap.
	*/
	template<typename T>
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
		TVector()
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
		}

		TVector(uint64 Capacity)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			ReAllocate(Capacity);
		}

		TVector(const TVector& Other)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			ReAllocate(Other.m_Size);
			m_Size = Other.m_Size;

			for (uint64 Index = 0; Index < Other.m_Size; Index++)
			{
				MemConstruct<T>(m_Data + Index, Other.m_Data[Index]);
			}
		}

		TVector(TVector&& other) noexcept
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
		{
			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
		}

		TVector(std::initializer_list<T> IntializerList)
			: m_Data(nullptr), m_Capacity(0), m_Size(0)
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

			MemConstruct<T>(m_Data + m_Size, element);

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

			MemConstruct<T>(m_Data + m_Size, std::move(element));

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

			MemConstruct<T>(m_Data + m_Size, std::forward<Args>(args)...);

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
		void Swap(TVector&& Vector)
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

		TVector& operator=(const TVector& Other)
		{
			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}

			if (m_Capacity < Other.m_Size)
			{
				ReAllocate(Other.m_Size);
			}

			for (uint64 Index = 0; Index < Other.m_Size; Index++)
			{
				MemConstruct<T>(m_Data + Index, Other.m_Data[Index]);
			}

			m_Size = Other.m_Size;

			return *this;
		}

		TVector& operator=(TVector&& Other) noexcept
		{
			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~T();
			}
			DeleteMemory();

			m_Data = Other.m_Data;
			m_Capacity = Other.m_Capacity;
			m_Size = Other.m_Size;

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
			m_Data = (T*)GMalloc->Alloc(NewCapacity * sizeof(T));
			m_Capacity = NewCapacity;
		}

		/*
		*
		*/
		void ReAllocateCopy(uint64 NewCapacity)
		{
			T* NewBlock = (T*)GMalloc->Alloc(NewCapacity * sizeof(T));

			for (uint64 Index = 0; Index < m_Size; Index++)
			{
				MemConstruct<T>(NewBlock + Index, Move(m_Data[Index]));
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
			GMalloc->Free(m_Data, m_Capacity * sizeof(T));
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
	};

}