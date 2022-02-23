// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"
#include "Apricot/Core/Assert.h"

namespace Apricot {

	template<typename T, uint64 S>
	class TArray
	{
	public:
		TArray()
		{
		}

	public:
		T& operator[](uint64 Index)
		{
			AE_DEBUG_CHECK(Index < S); // Array index out of range!
			return m_Data[Index];
		}

		const T& operator[](uint64 Index) const
		{
			AE_DEBUG_CHECK(Index < S); // Array index out of range!
			return m_Data[Index];
		}

	public:
		T* Data() const { return (T*)(&m_Data[0]); }
		uint64 Size() const { return S; }

	private:
		T m_Data[S];
	};

}