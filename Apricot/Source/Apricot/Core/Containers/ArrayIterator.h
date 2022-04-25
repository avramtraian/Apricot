// ArrayIterator.h - AVR Standard Template Library header

#pragma once

namespace Apricot {

	template<typename T>
	class ArrayIterator
	{
	public:
		ArrayIterator(T* Pointer)
			: m_Pointer(Pointer) {}

		bool operator==(const ArrayIterator& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool operator!=(const ArrayIterator& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}

		ArrayIterator& operator++()
		{
			m_Pointer++;
			return *this;
		}

		ArrayIterator operator++(int)
		{
			ArrayIterator Temp(m_Pointer);
			++(*this);
			return Temp;
		}

		ArrayIterator& operator--()
		{
			m_Pointer--;
			return *this;
		}

		ArrayIterator operator--(int)
		{
			ArrayIterator Temp(m_Pointer);
			--(*this);
			return Temp;
		}

		T* operator->() const
		{
			return m_Pointer;
		}

		T& operator*()
		{
			return *m_Pointer;
		}

	private:
		T* m_Pointer;
	};

}