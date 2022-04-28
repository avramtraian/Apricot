#pragma once

#include "Apricot/Core/Core.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"

#include "ArrayIterator.h"

namespace Apricot {

	template<typename T, SizeType S>
	class Array
	{
	public:
		using Iterator             = ArrayIterator<T>;
		using ConstIterator        = ArrayIterator<const T>;
		using ReverseIterator      = ArrayIterator<T>;
		using ReverseConstIterator = ArrayIterator<const T>;

	public:
		Array()
			: m_Data()
		{
		}

		Array(const Array& Other)
		{
			for (SizeType Index = 0; Index < S; Index++)
			{
				m_Data[Index] = Other.m_Data[Index];
			}
		}

		Array(Array&& Other) noexcept
		{
			for (SizeType Index = 0; Index < S; Index++)
			{
				m_Data[Index] = AE::Move(Other.m_Data[Index]);
			}
		}

		~Array()
		{
		}

	private:
		Array& operator=(const Array& Other)
		{
			for (SizeType Index = 0; Index < S; Index++)
			{
				m_Data[Index] = Other.m_Data[Index];
			}

			return *this;
		}

		Array& operator=(Array&& Other) noexcept
		{
			for (SizeType Index = 0; Index < S; Index++)
			{
				m_Data[Index] = AE::Move(Other.m_Data[Index]);
			}

			return *this;
		}

		T& operator[](SizeType Index)
		{
			AE_CORE_ASSERT(Index < S);
			return m_Data[Index];
		}

		const T& operator[](SizeType Index) const
		{
			AE_CORE_ASSERT(Index < S);
			return m_Data[Index];
		}

	public:
		constexpr T* Data() const { return (T*)(&m_Data[0]); }
		static constexpr SizeType Size() { return S; }

	public:
		Iterator begin()
		{
			return Iterator(Data());
		}

		Iterator end()
		{
			return Iterator(Data() + Size());
		}

		ConstIterator begin() const
		{
			return ConstIterator(Data());
		}

		ConstIterator end() const
		{
			return ConstIterator(Data() + Size());
		}

		ReverseIterator rbegin()
		{
			return ReverseIterator(Data() + Size() - 1);
		}

		ReverseIterator rend()
		{
			return ReverseIterator(Data() - 1);
		}

		ReverseConstIterator rbegin() const
		{
			return ReverseConstIterator(Data() + Size() - 1);
		}

		ReverseConstIterator rend() const
		{
			return ReverseConstIterator(Data() - 1);
		}

	private:
		T m_Data[S];
	};

}