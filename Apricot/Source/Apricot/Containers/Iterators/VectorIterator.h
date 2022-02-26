// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

namespace Apricot {

	template<typename T>
	class TVectorIterator
	{
	public:
		TVectorIterator(T* Pointer)
			: m_Pointer(Pointer) {}

	public:
		bool operator==(const TVectorIterator& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool operator!=(const TVectorIterator& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}

		TVectorIterator& operator++()
		{
			m_Pointer++;
			return *this;
		}

		TVectorIterator operator++(int)
		{
			TVectorIterator Temp = *this;
			m_Pointer++;
			return Temp;
		}

		TVectorIterator& operator--()
		{
			m_Pointer--;
			return *this;
		}

		TVectorIterator operator--(int)
		{
			TVectorIterator Temp = *this;
			m_Pointer--;
			return Temp;
		}

		T& operator*()
		{
			return *m_Pointer;
		}

		T& operator[](uint64 Index)
		{
			return *(m_Pointer + Index);
		}

		T* operator->()
		{
			return m_Pointer;
		}

	public:
		FORCEINLINE T* Get() const { return m_Pointer; }

	private:
		T* m_Pointer;
	};
	
}