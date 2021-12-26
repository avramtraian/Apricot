#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"

namespace Apricot {

	template<typename T>
	class TString
	{
	public:
		TString()
		{
			ClearTidy();
		}

		TString(const TString& other)
		{
			ReAllocate(other.m_Size);
			Memory::Copy(m_Data, other.m_Data, m_Size * sizeof(T));
			m_Size = other.m_Size;
		}

		TString(TString&& other) noexcept
		{
			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
		}

		TString(const T* other)
		{
			AE_CORE_ASSERT(other != nullptr, "Invalid argument!");
			m_Size = strlen(other) + 1;
			ReAllocate(m_Size);
			Memory::Copy(m_Data, other, m_Size * sizeof(T));
		}

		/*
		* Creates a TString with the first 'size' elements
		* If the provided raw string doesn't have the null-terminated character, it will allocate size + 1 elements
		*/
		TString(const T* other, uint64 size)
		{
			AE_CORE_ASSERT(other != nullptr && size > 0, "Invalid arguments!");
			
			if (other[size - 1] != nullptr)
			{
				m_Size = size + 1;
				ReAllocate(m_Size);
				Memory::Copy(m_Data, other, size * sizeof(T));
				m_Data[m_Size - 1] = 0;
			}
			else
			{
				m_Size = size;
				ReAllocate(m_Size);
				Memory::Copy(m_Data, other, size * sizeof(T));
			}
		}

		TString(uint64 capacity)
		{
			AE_CORE_ASSERT(capacity > 0, "Invalid argument!");
			ReAllocate(capacity);
			Clear();
		}

		~TString()
		{
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::String);
		}

	public:
		TString<T>& operator=(const TString<T>& other)
		{
			if (m_Capacity < other.m_Size)
			{
				ReAllocate(other.m_Size);
			}

			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(T));
			m_Size = other.m_Size;
			return *this;
		}

		TString<T>& operator=(TString<T>&& other) noexcept
		{
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::String);

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;
			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
			return *this;
		}

		TString<T>& operator=(const T* other)
		{
			uint64 size = strlen(other) + 1;

			if (m_Capacity < size)
			{
				ReAllocate(size);
			}

			Memory::Copy(m_Data, other, size * sizeof(T));
			m_Size = size;

			return *this;
		}

		const T& operator[](uint64 index) const
		{
			AE_CORE_ASSERT(index < m_Size, "Index out of range!");
			return m_Data[index];
		}

		T& operator[](uint64 index)
		{
			AE_CORE_ASSERT(index < m_Size, "Index out of range!");
			return m_Data[index];
		}

	public:
		inline T* Data() const { return m_Data; }
		inline uint64 Capacity() const { return m_Capacity; }
		inline uint64 Size() const { return m_Size; }

		/*
		* Set the size to 1 (null-terminating character only).
		* It doesn't actually deletes the data, or modifies the capacity of the string.
		*/
		void Clear()
		{
			m_Data[0] = 0;
			m_Size = 1;
		}

		/*
		* It removes all unused characters of the buffer.
		* It will reallocate the buffer.
		*/
		void Tidy()
		{
			ReAllocateCopy(m_Size);
		}

		/*
		* Set the size to 1 (null-terminating character only).
		* It will allocate a new buffer (with the capacity of 1).
		*/
		void ClearTidy()
		{
			ReAllocate(1);
			m_Data[0] = 1;
		}

		/*
		* It will resize the string to the specified value.
		* The newSize includes the null-terminating character as well.
		* It will reallocate the buffer only when the newSize is bigger than the existing buffer capacity.
		* 
		* @param newSize The size (in elements) of the new string. It includes the null-terminating character.
		*/
		void Resize(uint64 newSize)
		{
			if (newSize > m_Capacity)
			{
				ReAllocateCopy(newSize);
			}
			m_Size = newSize;
			m_Data[m_Size - 1] = 0;
		}

		/*
		* It will allocate a new buffer with the specified capacity.
		* 
		* @param newCapacity The capacity (in elements) of the new buffer.
		*/
		void Reserve(uint64 newCapacity)
		{
			ReAllocateCopy(newCapacity);
		}

		/*
		* It will append the specified string to the end.
		* Will cause a reallocation if the existing buffer isn't big enough.
		* 
		* @param string The string to append.
		*/
		TString<T>& Append(const TString<T>& string)
		{
			Impl_Append(string.Data(), string.Size());
			return *this;
		}

		/*
		* It will append the specified string to the end.
		* Will cause a reallocation if the existing buffer isn't big enough.
		*
		* @param string The C-Style string to append
		*/
		TString<T>& Append(const T* string)
		{
			Impl_Append(string, strlen(string));
			return *this;
		}

		/*
		* It will append the specified string to the end.
		* Will cause a reallocation if the existing buffer isn't big enough.
		*
		* @param string The C-Style string to append.
		* @param stringSize The size (in elements) of the string.
		*/
		TString<T>& Append(const T* string, uint64 stringSize)
		{
			Impl_Append(string, stringSize);
			return *this;
		}

	public:
		template<typename... Args>
		static TString<T> Format(const TString<T>& string, Args&&... args)
		{
			TString<T> formatted(string.Size());



			return formatted;
		}

	private:
		/*
		* Internal usage only.
		* It just allocates a new block of memory.
		* 
		* @param newCapacity The size (in elements) of the new block of memory.
		*/
		void ReAllocate(uint64 newCapacity)
		{
			T* newBlock = (T*)Memory::Allocate(newCapacity * sizeof(T), Memory::AllocTag::String);
			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::String);

			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

		/*
		* Internal usage only.
		* It allocates a new block of memory.
		* Copies as much as possible of the old block of memory into the new buffer.
		*
		* @param newCapacity The size (in elements) of the new block of memory.
		*/
		void ReAllocateCopy(uint64 newCapacity)
		{
			T* newBlock = (T*)Memory::Allocate(newCapacity * sizeof(T), Memory::AllocTag::String);

			if (m_Size > newCapacity)
			{
				m_Size = newCapacity;
			}

			Memory::Copy(newBlock, m_Data, m_Size * sizeof(T));
			newBlock[newCapacity - 1] = 0;

			Memory::Free(m_Data, m_Capacity * sizeof(T), Memory::AllocTag::String);
			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

		/*
		* Internal usage only.
		* Appends a C-Style string to the end.
		* 
		* @param string The C-Style string to append.
		* @param stringSize The size (in elements) of the string.
		*/
		void Impl_Append(const T* string, uint64 stringSize)
		{
			if (m_Size + stringSize - 1 > m_Capacity)
			{
				ReAllocateCopy(m_Size + stringSize - 1);
			}
			Memory::Copy(m_Data + m_Size - 1, string, stringSize * sizeof(T));
			m_Size += stringSize - 1;
		}

	private:
		/*
		* The buffer of the string.
		*/
		T* m_Data = nullptr;

		/*
		* The capacity of the buffer (in elements).
		*/
		uint64 m_Capacity = 0;

		/*
		* The size of the string (in elements).
		* It includes the null-terminating character.
		*/
		uint64 m_Size = 0;
	};

	using String = TString<char>;
	using WString = TString<wchar_t>;

	template<typename T, uint64 S>
	class TStackString
	{
	public:
		TStackString()
		{
			m_Data[0] = 0;
			m_Size = 1;
		}

		template<uint64 Q>
		TStackString(const TStackString<T, Q>& other)
		{
			STATIC_ASSERT(S >= T, "Possible TStackString buffer overflow!");

			m_Size = other.m_Size;
			Memory::Copy(m_Data, other.m_Data, m_Size * sizeof(T));
		}

		TStackString(const T* string)
		{
			// + null-terminating character
			m_Size = strlen(string) + 1;
			AE_CORE_VERIFY(m_Size <= S, "Overflowing the TStackString buffer!");

			Memory::Copy(m_Data, string, m_Size * sizeof(T));
		}

		TStackString(const T* string, uint64 size)
		{
			m_Size = size;
			AE_CORE_VERIFY(m_Size <= S, "Overflowing the TStackString buffer!");

			Memory::Copy(m_Data, string, m_Size * sizeof(T));
		}

	public:
		template<uint64 Q>
		TStackString<T, S>& operator=(const TStackString<T, Q>& other)
		{
			STATIC_ASSERT(S >= T, "Possible TStackString buffer overflow!");

			m_Size = other.m_Size;
			Memory::Copy(m_Data, other.m_Data, m_Size * sizeof(T));
			return *this;
		}

		const T& operator[](uint64 index) const
		{
			AE_CORE_ASSERT(index < S, "Index out of range!");
			return m_Data[index];
		}

		T& operator[](uint64 index)
		{
			AE_CORE_ASSERT(index < S, "Index out of range!");
			return m_Data[index];
		}

	public:
		inline T* Data() const { return m_Data; }
		inline uint64 MaxSize() const { return S; }
		inline uint64 Size() const { return m_Size; }

	public:
		template<typename... Args>
		static TStackString<T, S> Format(const TStackString<T, S>& string, Args&&... args)
		{
			TStackString<T, S> formatted;

			bool insideArg = false;
			uint64 argBegin = 0;
			uint64 argEnd = 0;
			uint64 argSize = 0;
			uint64 formattedOffset = 0;

			for (uint64 index = 0; index < string.Size(); index++)
			{
				const T character = string[index];
				if (character == '{')
				{
					argBegin = index;
					insideArg = true;
				}
				else if (character == '}')
				{
					argEnd = index + 1;
					argSize = argEnd - argBegin;
					insideArg = false;
				}
				else if (!insideArg)
				{
					formatted[formattedOffset] = character;
					formattedOffset++;
				}
			}

			return formatted;
		}

	private:

	private:
		T m_Data[S] = { 0 };
		// Includes the null-terminating character
		uint64 m_Size = 0;
	};

	// String allocated completely on the stack
	using String16 = TStackString<char, 16>;
	using String32 = TStackString<char, 32>;
	using String64 = TStackString<char, 64>;
	using String128 = TStackString<char, 128>;

	using WString16 = TStackString<wchar_t, 16>;
	using WString32 = TStackString<wchar_t, 32>;
	using WString64 = TStackString<wchar_t, 64>;
	using WString128 = TStackString<wchar_t, 128>;

}