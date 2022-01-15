// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"

#define AE_STRING_NPOS static_cast<uint64>(-1)

namespace Apricot {

	template<typename T>
	inline uint64 CStrLength(const T* string)
	{
		AE_STATIC_ASSERT(false, "");
	}

	template<>
	inline uint64 CStrLength(const char8* string)
	{
		return (uint64)strlen(string);
	}

	template<>
	inline uint64 CStrLength(const char16* string)
	{
		return (uint64)wcslen(string);
	}

	template<typename T>
	struct THStringAlias
	{
		T* Data = nullptr;
		uint64 Capacity = 0;
		uint64 Size = 0;
	};

	template<typename T, uint64 S>
	struct THStackStringAlias
	{
		T Data[S] = { 0 };
		uint64 Size = 0;
	};

	template<typename T>
	struct THStringViewAlias
	{
		const T* String = nullptr;
		uint64 Size = 0;
	};

	template<typename T>
	class TString;

	template<typename T, uint64 S>
	class TStackString;

	template<typename T>
	class TStringView;

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
			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(T));
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
			m_Size = CStrLength(other) + 1;
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

		template<uint64 S>
		TString(const TStackString<T, S>& stack)
		{
			const THStackStringAlias<T, S>* alias = reinterpret_cast<const THStackStringAlias<T, S>*>(&stack);

			ReAllocate(alias->Size);
			Memory::Copy(m_Data, alias->Data, alias->Size * sizeof(T));
			m_Size = alias->Size;
		}

		TString<T>(const TStringView<T>& view)
		{
			const THStringViewAlias<T>* alias = reinterpret_cast<const THStringViewAlias<T>*>(&view);

			if (m_Capacity < alias->Size)
			{
				ReAllocate(alias->Size);
			}

			Memory::Copy(m_Data, alias->String, alias->Size * sizeof(T));
			m_Size = alias->Size;
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
			uint64 size = CStrLength(other) + 1;

			if (m_Capacity < size)
			{
				ReAllocate(size);
			}

			Memory::Copy(m_Data, other, size * sizeof(T));
			m_Size = size;

			return *this;
		}

		template<uint64 S>
		TString<T>& operator=(const TStackString<T, S>& stack)
		{
			const THStackStringAlias<T, S>* alias = reinterpret_cast<const THStackStringAlias<T, S>*>(&stack);

			ReAllocate(alias->Size);
			Memory::Copy(m_Data, alias->Data, alias->Size * sizeof(T));
			m_Size = alias->Size;
			return *this;
		}

		TString<T>& operator=(const TStringView<T>& view)
		{
			const THStringViewAlias<T>* alias = reinterpret_cast<const THStringViewAlias<T>*>(&view);

			if (m_Capacity < alias->Size)
			{
				ReAllocate(alias->Size);
			}

			Memory::Copy(m_Data, alias->String, alias->Size * sizeof(T));
			m_Size = alias->Size;
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

		inline const T* c_str() const { return m_Data; }

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
			Impl_Append(string, CStrLength(string));
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

		friend class StringFormatter;
	};

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
			AE_STATIC_ASSERT(S >= Q, "Possible TStackString buffer overflow!");

			m_Size = other.Size();
			Memory::Copy(m_Data, other.Data(), m_Size * sizeof(T));
		}

		TStackString(const T* string)
		{
			// + null-terminating character
			uint64 stringSize = CStrLength(string) + 1;
			AE_CORE_VERIFY_RETURN(stringSize <= S, AE_VOID, "Overflowing the TStackString buffer!");

			m_Size = stringSize;
			Memory::Copy(m_Data, string, m_Size * sizeof(T));
		}

		TStackString(const T* string, uint64 size)
		{
			m_Size = size;
			AE_CORE_VERIFY(m_Size <= S, "Overflowing the TStackString buffer!");

			Memory::Copy(m_Data, string, m_Size * sizeof(T));
		}

		TStackString(const TString<T>& string)
		{
			const THStringAlias<T>* alias = reinterpret_cast<const THStringAlias<T>*>(&string);
			AE_CORE_VERIFY_RETURN(alias->Size <= S, AE_VOID, "TStackString buffer overflow!");

			m_Size = alias->Size;
			Memory::Copy(m_Data, alias->Data, m_Size * sizeof(T));
		}

		TStackString(const TStringView<T>& view)
		{
			const THStringViewAlias<T>* alias = reinterpret_cast<const THStringViewAlias<T>*>(&view);
			AE_CORE_VERIFY_RETURN(alias->Size <= S, AE_VOID, "TStackString buffer overflow!");

			m_Size = alias->Size;
			Memory::Copy(m_Data, alias->String, m_Size * sizeof(T));
		}

	public:
		template<uint64 Q>
		TStackString<T, S>& operator=(const TStackString<T, Q>& other)
		{
			AE_STATIC_ASSERT(S >= T, "Possible TStackString buffer overflow!");

			m_Size = other.m_Size;
			Memory::Copy(m_Data, other.m_Data, m_Size * sizeof(T));
			return *this;
		}

		TStackString<T, S>& operator=(const TString<T>& string)
		{
			const THStringAlias<T>* alias = reinterpret_cast<const THStringAlias<T>*>(&string);
			AE_CORE_VERIFY_RETURN(alias->Size <= S, *this, "TStackString buffer overflow!");

			m_Size = alias->Size;
			Memory::Copy(m_Data, alias->Data, m_Size * sizeof(T));
			return *this;
		}

		TStackString<T, S>& operator=(const TStringView<T>& view)
		{
			const THStringViewAlias<T>* alias = reinterpret_cast<const THStringViewAlias<T>*>(&view);
			AE_CORE_VERIFY_RETURN(alias->Size <= S, *this, "TStackString buffer overflow!");

			m_Size = alias->Size;
			Memory::Copy(m_Data, alias->String, m_Size * sizeof(T));
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

		bool operator==(const T* other) const
		{
			return Impl_Equals(other, CStrLength(other) + 1);
		}

		bool operator!=(const T* other) const
		{
			return !Impl_Equals(other, CStrLength(other) + 1);
		}

	public:
		inline T* Data() const { return const_cast<T*>(&m_Data[0]); }
		inline uint64 MaxSize() const { return S; }
		inline uint64 Size() const { return m_Size; }

		void Impl_SetSize(uint64 newSize) { m_Size = newSize; }

		inline const T* c_str() const { return m_Data; }

		T PushBack(T character)
		{
			AE_CORE_ASSERT(m_Size < S, "TStackString buffer overflow!")
			m_Data[m_Size - 1] = character;
			m_Data[m_Size] = 0;
			m_Size++;
			return character;
		}

		TStackString<T, S> ToLowerCase() const
		{
			TStackString<T, S> lowerCase;
			lowerCase.m_Size = m_Size;

			static uint64 difference = (uint64)'a' - (uint64)'A';
			for (uint64 index = 0; index < m_Size; index++)
			{
				if ('A' <= m_Data[index] && m_Data[index] <= 'Z')
				{
					lowerCase[index] = m_Data[index] + (T)difference;
				}
				else
				{
					lowerCase[index] = m_Data[index];
				}
			}

			return lowerCase;
		}

		uint64 Find(const T* search, uint64 intervalFirst = 0, uint64 intervalLast = 0) const
		{
			return Impl_Find(search, CStrLength(search), intervalFirst, intervalLast);
		}

		TStackString<T, S> Substr(uint64 first, uint64 last = 0) const
		{
			if (last == 0)
			{
				last = m_Size;
			}
			AE_CORE_ASSERT_RETURN(first < last && last <= m_Size, {}, "Invalid parameters!");
			
			TStackString<T, S> substr;
			substr.m_Size = last - first;
			Memory::Copy(substr.m_Data, m_Data + first, substr.m_Size);

			if (substr.m_Data[substr.m_Size - 1] != 0)
			{
				substr.m_Size++;
				substr.m_Data[substr.m_Size - 1] = 0;
			}

			return substr;
		}

		template<typename Func>
		void ForEachWord(Func callback) const
		{
			if (m_Size < 2)
			{
				return;
			}

			int64 begin = -1;
			for (uint64 index = 0; index < m_Size; index++)
			{
				if (m_Data[index] == ' ' || m_Data[index] == 0)
				{
					uint64 wordSize = index - begin;
					if (wordSize > 1 || m_Data[begin] != ' ')
					{
						TStackString<T, S> word = Substr(begin + 1, index);
						callback(word);
					}

					begin = index;
				}
			}
		}

	private:
		bool Impl_Equals(const T* other, uint64 otherSize) const
		{
			if (m_Size != otherSize)
			{
				return false;
			}

			for (uint64 index = 0; index < m_Size; index++)
			{
				if (m_Data[index] != other[index])
				{
					return false;
				}
			}

			return true;
		}

		uint64 Impl_Find(const T* search, uint64 searchSize, uint64 intervalFirst, uint64 intervalLast) const
		{
			if (intervalLast == 0)
			{
				intervalLast = m_Size;
			}

			AE_CORE_ASSERT_RETURN(intervalFirst < intervalLast && intervalLast <= m_Size, AE_STRING_NPOS, "Invalid parameters!");

			for (uint64 offset = intervalFirst; offset < intervalLast - searchSize; offset++)
			{
				bool8 bWasFound = true;
				for (uint64 index = 0; index < searchSize; index++)
				{
					if (m_Data[offset + index] != search[index])
					{
						bWasFound = false;
						break;
					}
				}

				if (bWasFound)
				{
					return offset;
				}
			}

			return AE_STRING_NPOS;
		}

	private:
		T m_Data[S] = { 0 };
		// Includes the null-terminating character
		uint64 m_Size = 0;

		friend class StringFormatter;
	};

	template<typename T>
	class TStringView
	{
	public:
		TStringView()
		{

		}

		TStringView(const TStringView<T>& other)
		{
			m_String = other.m_String;
			m_Size = other.m_Size;
		}

		TStringView(const T* string)
		{
			m_String = string;
			m_Size = CStrLength(m_String) + 1;
		}

		TStringView(const T* string, uint64 size)
		{
			m_String = string;
			m_Size = size;
		}

		TStringView(const TString<T>& string)
		{
			const THStringAlias<T>* alias = reinterpret_cast<const THStringAlias<T>*>(&string);
			m_String = alias->Data;
			m_Size = alias->Size;
		}

		template<uint64 S>
		TStringView(const TStackString<T, S>& stack)
		{
			const THStackStringAlias<T, S>* alias = reinterpret_cast<const THStackStringAlias<T, S>*>(&stack);
			m_String = alias->Data;
			m_Size = alias->Size;
		}

	public:
		TStringView<T>& operator=(const TStringView<T>& other)
		{
			m_String = other.m_String;
			m_Size = other.m_Size;
			return *this;
		}

		TStringView<T>& operator=(const T* string)
		{
			m_String = string;
			m_Size = CStrLength(string) + 1;
			return *this;
		}

		TStringView<T>& operator=(const TString<T>& string)
		{
			const THStringAlias<T>* alias = reinterpret_cast<const THStringAlias<T>*>(&string);
			m_String = alias->Data;
			m_Size = alias->Size;
			return *this;
		}

		template<uint64 S>
		TStringView<T>& operator=(const TStackString<T, S>& stack)
		{
			const THStackStringAlias<T, S>* alias = reinterpret_cast<const THStackStringAlias<T, S>*>(&stack);
			m_String = alias->Data;
			m_Size = alias->Size;
			return *this;
		}

		const T& operator[](uint64 index) const
		{
			AE_CORE_ASSERT(index < m_Size, "Index out of range!");
			return m_String[index];
		}

	public:
		const T* String() const { return m_String; }
		uint64 Size() const { return m_Size; }

		inline const T* c_str() const { return m_String; }

	private:
		const T* m_String = nullptr;
		uint64 m_Size = 0;
	};

	// Strings allocated on the heap
	using String = TString<char8>;
	using WString = TString<char16>;

	// Strings allocated completely on the stack
	using String16		= TStackString<char8, 16>;
	using String32		= TStackString<char8, 32>;
	using String64		= TStackString<char8, 64>;
	using String128		= TStackString<char8, 128>;
	using String256     = TStackString<char8, 256>;

	using WString16		= TStackString<char16, 16>;
	using WString32		= TStackString<char16, 32>;
	using WString64		= TStackString<char16, 64>;
	using WString128	= TStackString<char16, 128>;
	using WString256	= TStackString<char16, 256>;

	// StringViews
	using StringView	= TStringView<char8>;
	using WStringView	= TStringView<char16>;

}