// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Memory/Allocator.h"

namespace Apricot {

	template<typename T, typename Alloc = AGlobalAllocator>
	class TString
	{
	public:
		TString(Alloc* Allocator = nullptr)
		{
			m_Data = nullptr;
			m_Capacity = 0;
			m_Size = 0;

			if (Allocator == nullptr)
			{
				if (Alloc::GetStaticType() == EAllocatorType::Global)
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
				m_Allocator = Allocator;
			}
		}

	public:
		T& operator[](uint64 Index)
		{
			AE_DEBUG_CHECK(Index < m_Capacity);
			return m_Data[Index];
		}

		const T& operator[](uint64 Index) const
		{
			AE_DEBUG_CHECK(Index < m_Capacity);
			return m_Data[Index];
		}

	public:
		T* Data() const { return m_Data; }
		uint64 Capacity() const { return m_Size; }
		uint64 Size() const { return m_Size; }
		const T* c_str() const { return m_Data; }

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

	using AString = TString<char8>;
	using AWString = TString<char16>;

}