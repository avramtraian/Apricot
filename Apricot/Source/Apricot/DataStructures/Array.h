// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Core/Assert.h"

namespace Apricot {

	template<typename T, uint64 S>
	class TArray
	{
	public:
		TArray()
		{
			
		}

		TArray(const TArray<T, S>& other)
		{
			for (uint64 index = 0; index < S; index++)
			{
				m_Data[index] = other.m_Data[index];
			}
		}

		TArray(TArray<T, S>&& other) noexcept
		{
			for (uint64 index = 0; index < S; index++)
			{
				m_Data[index] = std::move(other.m_Data[index]);
			}
		}

		~TArray()
		{
			
		}

	public:
		TArray<T, S>& operator=(const TArray<T, S>& other)
		{
			for (uint64 index = 0; index < S; index++)
			{
				m_Data[index] = other.m_Data[index];
			}
			return *this;
		}

		TArray<T, S>& operator=(TArray<T, S>&& other) noexcept
		{
			for (uint64 index = 0; index < S; index++)
			{
				m_Data[index] = std::move(other.m_Data[index]);
			}
			return *this;
		}

		const T& operator[](uint64 index) const
		{
			AE_CORE_ASSERT(index < S, "Index out of range");
			return m_Data[index];
		}
		
		T& operator[](uint64 index)
		{
			AE_CORE_ASSERT(index < S, "Index out of range");
			return m_Data[index];
		}

	public:
		T* Data() const { return (T*)(&m_Data[0]); }
		uint64 Size() const { return S; }

	private:
		T m_Data[S];
	};

}