// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Iterators/VectorIterator.h"

#include "Apricot/Core/Assert.h"

namespace Apricot {

	template<typename T, uint64 S>
	class TArray
	{
	public:
		using TIterator             = TVectorIterator<T>;
		using TConstIterator        = TVectorIterator<const T>;
		using TReverseIterator      = TVectorIterator<T>;
		using TReverseConstIterator = TVectorIterator<const T>;

	public:
		TArray()
		{
		}

	public:
		T& operator[](uint64 Index)
		{
			AE_CORE_ASSERT(Index < S); // Array index out of range!
			return m_Data[Index];
		}

		const T& operator[](uint64 Index) const
		{
			AE_CORE_ASSERT(Index < S); // Array index out of range!
			return m_Data[Index];
		}

	public:
		TIterator begin()
		{
			return TIterator(m_Data);
		}

		TIterator end()
		{
			return TIterator(m_Data + S);
		}

		TConstIterator begin() const
		{
			return TConstIterator(m_Data);
		}

		TConstIterator end() const
		{
			return TConstIterator(m_Data + S);
		}

		TReverseIterator rbegin()
		{
			return TReverseIterator(m_Data + S - 1);
		}

		TReverseIterator rend()
		{
			return TReverseIterator(m_Data - 1);
		}

		TReverseConstIterator rbegin() const
		{
			return TReverseConstIterator(m_Data + S - 1);
		}

		TReverseConstIterator rend() const
		{
			return TReverseConstIterator(m_Data - 1);
		}

	public:
		FORCEINLINE T*     Data() const { return (T*)(&m_Data[0]); }
		FORCEINLINE uint64 Size() const { return S; }

	private:
		T m_Data[S];
	};

}