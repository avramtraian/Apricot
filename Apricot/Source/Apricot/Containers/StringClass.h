// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Memory/Allocator.h"

namespace Apricot {

	template<typename T, typename Alloc = GlobalAllocator>
	class TString
	{
	public:
		TString(Alloc* allocator = nullptr)
		{
			m_Data = nullptr;
			m_Capacity = 0;
			m_Size = 0;

			if (allocator == nullptr)
			{
				if (Alloc::GetStaticType() == AllocatorType::Global)
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
				m_Allocator = allocator;
			}
		}

	public:
		T& operator[](uint64 index)
		{
			AE_DEBUG_CHECK(index < m_Capacity);
			return m_Data[index];
		}

		const T& operator[](uint64 index) const
		{
			AE_DEBUG_CHECK(index < m_Capacity);
			return m_Data[index];
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

	using String = TString<char8>;
	using WString = TString<char16>;

}