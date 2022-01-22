#pragma once

#include "Apricot/Core/Base.h"
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
		T& operator[](uint64 index)
		{
			AE_BASE_ASSERT(index < S, "Array index out of range!");
			return m_Data[index];
		}

		const T& operator[](uint64 index) const
		{
			AE_BASE_ASSERT(index < S, "Array index out of range!");
			return m_Data[index];
		}

	public:
		T* Data() const { return (T*)(&m_Data[0]); }
		uint64 Size() const { return S; }

	private:
		T m_Data[S];
	};

}