// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Log.h"

#include "Apricot/Core/Memory/ApricotMemory.h"
#include "Apricot/Core/Memory/ApricotAllocator.h"
#include "Apricot/Core/Memory/HeapAllocator.h"

#include "String.h"

namespace Apricot {

	/**
	* Direction from where the searching begins
	*/
	enum class ESearchDirection : uint8
	{
		None = 0,

		FromEnd,
		FromBeginning,

		MaxEnumValue,
	};

	/**
	* Specification of the characters matching
	*/
	enum class ESearchCase : uint8
	{
		None = 0,

		// Doesn't matter if chars are upper or lower case, they match
		Ignore,

		// Must have the same char value
		CaseSensitive,

		MaxEnumValue,
	};

	namespace Char {

		template<typename CharType>
		CharType ToLowerCase(CharType character)
		{
			if ('A' <= character && character <= 'Z')
			{
				return character + ('a' - 'A');
			}
			return character;
		}

		template<typename CharType>
		CharType ToUpperCase(CharType character)
		{
			if ('a' <= character && character <= 'z')
			{
				return character - ('a' - 'A');
			}
			return character;
		}

	}

	// Forward declarations
	template<typename CharType, uint64 SSOBufferSize, typename AllocatorType>
	class TString;

	/**
	* C++ Core Engine Architecture
	* 
	* 
	*/
	template<typename CharType>
	class TStringView
	{
	public:
		static constexpr uint64 InvalidPos = static_cast<uint64>(-1);

	public:
		// All function bodies are at the bottom of the file
		template<uint64 SSOBufferSize, typename AllocatorType>
		TStringView(const TString<CharType, SSOBufferSize, AllocatorType>& string);

		TStringView(const CharType* string);

		TStringView(const CharType* string, uint64 stringSize);

	public:
		FORCEINLINE bool            IsEmpty() const { return m_Size <= 1; }
		FORCEINLINE uint64          Size()    const { return m_Size; }
		FORCEINLINE const CharType* Data()    const { return m_Data; }
		FORCEINLINE const CharType* c_str()   const { return m_Data; }
		
	private:
		const CharType* m_Data = nullptr;
		uint64 m_Size = 0;
	};

	/**
	* C++ Core Engine Architecture
	* 
	* @tparam CharType Type of used characters
	* @tparam SSOBufferSize Size, in elements, of the stack-allocated small string buffer optimization
	* @tparam AllocatorType Type of the allocator used to manipulate the memory
	*/
	template<typename CharType, uint64 SSOBufferSize, typename AllocatorType = HeapAllocator>
	class TString
	{
	public:
		static constexpr uint64 InvalidPos = static_cast<uint64>(-1);

	public:
		TString()
		{
			m_Allocator = AllocatorType::GetDefault();
			m_Size = 1;
			m_SSO[0] = 0;
		}

		explicit TString(AllocatorType* allocator)
			: m_Allocator(allocator)
		{
			m_Size = 1;
			m_SSO[0] = 0;
		}

		template<uint64 OtherSSOBufferSize, typename OtherAllocatorType>
		TString(const TString<CharType, OtherSSOBufferSize, OtherAllocatorType>& other)
		{
			m_Allocator = AllocatorType::GetDefault();
			m_Size = other.m_Size;

			if (IsHeapData())
			{
				ReAllocate(other.m_Size);
				MemCpy(m_Data, other.Data(), other.m_Size * sizeof(CharType));
			}
			else
			{
				MemCpy(m_SSO, other.Data(), other.m_Size * sizeof(CharType));
			}
		}

		template<uint64 OtherSSOBufferSize, typename OtherAllocatorType>
		TString(TString<CharType, OtherSSOBufferSize, OtherAllocatorType>&& other) noexcept
		{
			m_Allocator = AllocatorType::GetStaticType() == OtherAllocatorType::GetStaticType() ? (AllocatorType*)other.m_Allocator : AllocatorType::GetDefault();
			m_Size = Move(other.m_Size);

			if (!other.IsUsingSSO())
			{
				if (IsHeapData())
				{
					// The allocator will be always the same
					m_Data = Move(other.m_Data);
					m_Capacity = Move(other.m_Capacity);

					if (m_Allocator->GetUUID() == other.m_Allocator->GetUUID())
					{
						m_Data = Move(other.m_Data);
						m_Capacity = Move(other.m_Capacity);
					}
					else
					{
						ReAllocate(other.m_Size);
						MemCpy(m_Data, other.m_Data, other.m_Size * sizeof(CharType));
						other.DeleteData();
					}
				}
				else
				{
					// We don't have any memory to free first
					MemCpy(m_SSO, other.m_Data, other.m_Size * sizeof(CharType));
					other.DeleteData();
				}
				other.m_Data = nullptr;
				other.m_Capacity = 0;
			}
			else
			{
				if (IsHeapData())
				{
					ReAllocate(other.m_Size);
					MemCpy(m_Data, other.m_SSO, other.m_Size * sizeof(CharType));
				}
				else
				{
					MemCpy(m_SSO, other.m_SSO, other.m_Size * sizeof(CharType));
				}
			}

			other.m_Size = 1;
			other.m_SSO[0] = 0;
		}

		TString(const TStringView<CharType>& stringView)
		{
			m_Allocator = AllocatorType::GetDefault();
			m_Size = stringView.Size();

			if (IsHeapData())
			{
				ReAllocate(stringView.Size());
				MemCpy(m_Data, stringView.Data(), stringView.Size() * sizeof(CharType));
			}
			else
			{
				MemCpy(m_SSO, stringView.Data(), stringView.Size() * sizeof(CharType));
			}
		}

		TString(const CharType* other)
		{
			m_Allocator = AllocatorType::GetDefault();

			uint64 otherSize = (uint64)StrLength(other) + 1;
			m_Size = otherSize;

			if (IsHeapData())
			{
				ReAllocate(otherSize);
				MemCpy(m_Data, other, otherSize * sizeof(CharType));
			}
			else
			{
				MemCpy(m_SSO, other, otherSize * sizeof(CharType));
			}
		}

		~TString()
		{
			if (m_Data)
			{
				DeleteData();
			}
		}

	public:
		FORCEINLINE uint64           Size()         const { return m_Size; }
		FORCEINLINE uint64           Capacity()     const { return m_Capacity; }
		FORCEINLINE constexpr uint64 SSOSize()      const { return SSOBufferSize; }
		FORCEINLINE CharType*        Data()         const { bool bSSO = IsUsingSSO(); return (CharType*)((uintptr)m_Data * (!bSSO) + (uintptr)m_SSO * (bSSO)); }
		FORCEINLINE CharType*        HeapData()     const { return m_Data; }
		FORCEINLINE CharType*        SSOData()      const { return m_SSO; }
		FORCEINLINE const CharType*  c_str()        const { return Data(); }
		FORCEINLINE bool             IsEmpty()      const { return m_Size <= 1; }
		FORCEINLINE bool             IsUsingSSO()   const { return m_Size <= SSOBufferSize; }
		FORCEINLINE bool             IsHeapData()   const { return m_Size > SSOBufferSize; }
		FORCEINLINE AllocatorType*   GetAllocator() const { return m_Allocator; }

	public:
		/**
		* It will cause a reallocation if string data is on the heap
		*/
		void SetAllocator(AllocatorType* newAllocator)
		{
			if (m_Data)
			{
				CharType* newBlock = (CharType*)newAllocator->Alloc(m_Size * sizeof(CharType), EAllocatorHint::String);
				MemCpy(newBlock, m_Data, m_Size * sizeof(CharType));
				DeleteData();
				m_Data = newBlock;
				m_Capacity = m_Size;
			}
			m_Allocator = newAllocator;
		}

	public:
		/**
		* Finds an instance of the substring sequence. Returns InvalidPos is none is found.
		* 
		* @param substring The substring to found
		* @param searchCase Searching character matching specification. See ESearchCase. Default is ESearchCase::CaseSensitive.
		* @param searchDirection Searching direction. See ESearchDirection. Default is ESearchDirection::FromBeginning.
		* @param searchOffsetBeginning Offset, from the beginning, of the search area.
		* @param searchOffsetEnd Offset, from the end, of the search area.
		* 
		* @returns The index of the first-found instance of the given substring. Returns InvalidPos is none is found.
		*/
		NODISCARD uint64 Find(const TStringView<CharType>& substring, ESearchCase searchCase = ESearchCase::CaseSensitive, ESearchDirection searchDirection = ESearchDirection::FromBeginning, uint64 searchOffsetBeginning = 0, uint64 searchOffsetEnd = 0) const
		{
			const CharType* data = Data();

			// TODO (Avr): Think about the performance implications of this method.
			//                 Maybe rewrite it?
			bool(*PFN_CompareCharacters)(CharType c1, CharType c2) = nullptr;
			switch (searchCase)
			{
				case ESearchCase::Ignore:
					PFN_CompareCharacters = [](CharType c1, CharType c2) -> bool { return (Char::ToLowerCase(c1) == Char::ToLowerCase(c2)); };
					break;
				case ESearchCase::CaseSensitive:
					PFN_CompareCharacters = [](CharType c1, CharType c2) -> bool { return c1 == c2; };
					break;
				default:
					AE_CORE_ASSERT_NO_ENTRY();
					break;
			}

			switch(searchDirection)
			{
				case ESearchDirection::FromBeginning:
				{
					int64 maxStringIndex = (int64)m_Size - (int64)substring.Size() - (int64)searchOffsetEnd;
					for (int64 stringIndex = searchOffsetBeginning; stringIndex <= maxStringIndex; stringIndex++)
					{
						bool bFound = true;
						for (uint64 substringIndex = 0; substringIndex < substring.Size() - 1; substringIndex++)
						{
							if (!PFN_CompareCharacters(data[stringIndex + substringIndex], substring.Data()[substringIndex]))
							{
								bFound = false;
								break;
							}
						}
						if (bFound)
						{
							return stringIndex;
						}
					}
				}
				case ESearchDirection::FromEnd:
				{
					for (int64 stringIndex = m_Size - substring.Size() - searchOffsetEnd; stringIndex >= (int64)searchOffsetBeginning; stringIndex--)
					{
						bool bFound = true;
						for (uint64 substringIndex = 0; substringIndex < substring.Size() - 1; substringIndex++)
						{
							if (!PFN_CompareCharacters(data[stringIndex + substringIndex], substring.Data()[substringIndex]))
							{
								bFound = false;
								break;
							}
						}
						if (bFound)
						{
							return stringIndex;
						}
					}
				}
				default:
				{
					AE_CORE_ASSERT_NO_ENTRY();
					break;
				}
			}

			return InvalidPos;
		}

		NODISCARD TString Substr(uint64 where, uint64 size = 0) const
		{
			AE_CORE_ASSERT(where < m_Size - 1); // where out of range
			AE_CORE_ASSERT(where + size <= m_Size); // size too big

			CharType* substringData = Data() + where;

			if (size == 0 || size == m_Size - where)
			{
				TString substr = TString(m_Allocator);
				substr = TStringView<CharType>{ substringData, m_Size - where };
				return substr;
			}
			else
			{
				CharType tempChar = substringData[size];
				substringData[size] = 0;
				TStringView<CharType> stringView{ substringData, size + 1 };
				TString substr = TString(m_Allocator);
				substr = stringView;
				substringData[size] = tempChar;
				return substr;
			}
		}

		TString& Append(const TStringView<CharType>& substring)
		{
			CharType* data = m_SSO;

			if (m_Size + substring.Size() - 1 > SSOBufferSize)
			{
				if (m_Size > m_Capacity)
				{
					ReAllocateCopy(m_Size);
				}
				data = m_Data;
			}

			MemCpy(data + m_Size - 1, substring.Data(), substring.Size() * sizeof(CharType));
			m_Size = m_Size + substring.Size() - 1;

			return *this;
		}

		TString& Insert(const TStringView<CharType>& substring, uint64 where)
		{
			CharType* data = m_SSO;
			uint64 newSize = m_Size + substring.Size() - 1;
			
			if (newSize > SSOBufferSize)
			{
				if (newSize > m_Capacity)
				{
					ReAllocateCopy(newSize);
				}
				data = m_Data;
			}

			for (uint64 index = where; index < m_Size; index++)
			{
				data[index + substring.Size() - 1] = data[index];
			}

			MemCpy(data + where, substring.Data(), (substring.Size() - 1) * sizeof(CharType));

			m_Size = newSize;
			data[m_Size - 1] = 0;
			return *this;
		}

		bool Equals(const TStringView<CharType>& other, ESearchCase searchCase = ESearchCase::CaseSensitive) const
		{
			if (m_Size != other.m_Size)
			{
				return false;
			}

			CharType* data = Data();
			CharType* otherData = other.Data();

			switch (searchCase)
			{
				case ESearchCase::Ignore:
				{
					for (uint64 index = 0; index < m_Size - 1; index++)
					{
						if (Char::ToLowerCase(data[index]) != Char::ToLowerCase(otherData[index]))
						{
							return false;
						}
					}
					break;
				}
				case ESearchCase::CaseSensitive:
				{
					for (uint64 index = 0; index < m_Size - 1; index++)
					{
						if (data[index] != otherData[index])
						{
							return false;
						}
					}
					break;
				}
				default:
				{
					AE_CORE_ASSERT_NO_ENTRY();
					return false;
				}
			}

			return true;
		}

		template<typename NewCharType, typename NewAllocatorType = HeapAllocator, uint64 NewSSOBufferSize = SSOBufferSize>
		NODISCARD TString<NewCharType, NewSSOBufferSize, NewAllocatorType> As(NewAllocatorType* newAllocator = NewAllocatorType::GetDefault()) const
		{
			TString<NewCharType, NewSSOBufferSize, NewAllocatorType> newString{ newAllocator };
			newString.m_Size = m_Size;

			CharType* data = Data();
			if (newString.IsUsingSSO())
			{
				for (uint64 index = 0; index < m_Size; index++)
				{
					newString.m_SSO[index] = data[index];
				}
			}
			else
			{
				newString.ReAllocate(newString.m_Size);
				for (uint64 index = 0; index < m_Size; index++)
				{
					newString.m_Data[index] = data[index];
				}
			}

			return newString;
		}

		NODISCARD TString ToLowerCase(AllocatorType* allocator = nullptr) const
		{
			if (allocator == nullptr)
			{
				allocator = m_Allocator;
			}

			TString lowerCase = TString(allocator);
			lowerCase.m_Size = m_Size;
			CharType* lowerCaseData = lowerCase.m_SSO;

			if (lowerCase.IsHeapData())
			{
				lowerCase.ReAllocate(m_Size);
				lowerCaseData = lowerCase.m_Data;
			}

			for (uint64 index = 0; index < m_Size; index++)
			{
				lowerCaseData[index] = Char::ToLowerCase(m_Data[index]);
			}
			return lowerCase;
		}

		NODISCARD TString ToUpperCase(AllocatorType* allocator = nullptr) const
		{
			if (allocator == nullptr)
			{
				allocator = m_Allocator;
			}

			TString upperCase = TString(allocator);
			upperCase.m_Size = m_Size;
			CharType* lowerCaseData = upperCase.m_SSO;

			if (upperCase.IsHeapData())
			{
				upperCase.ReAllocate(m_Size);
				lowerCaseData = upperCase.m_Data;
			}

			for (uint64 index = 0; index < m_Size; index++)
			{
				lowerCaseData[index] = Char::ToUpperCase(m_Data[index]);
			}
			return upperCase;
		}

	public:
		template<uint64 OtherSSOBufferSize, typename OtherAllocatorType>
		TString& operator=(const TString<CharType, OtherSSOBufferSize, OtherAllocatorType>& other)
		{
			m_Size = other.m_Size;
			if (IsHeapData())
			{
				if (other.m_Size > m_Capacity)
				{
					ReAllocate(other.m_Size);
				}
				MemCpy(m_Data, other.Data(), other.m_Size * sizeof(CharType));
			}
			else
			{
				if (m_Data)
				{
					DeleteData();
					m_Data = nullptr;
					m_Capacity = 0;
				}
				MemCpy(m_SSO, other.Data(), other.m_Size * sizeof(CharType));
			}

			return *this;
		}

		template<uint64 OtherSSOBufferSize, typename OtherAllocatorType>
		TString& operator=(TString<CharType, OtherSSOBufferSize, OtherAllocatorType>&& other) noexcept
		{
			m_Size = Move(other.m_Size);
			if (!other.IsUsingSSO())
			{
				if (IsHeapData())
				{
					if (m_Allocator->GetUUID() == other.m_Allocator->GetUUID())
					{
						// Because we are not using SSO, we certainly have a heap data buffer
						DeleteData();
						m_Data = Move(other.m_Data);
						m_Capacity = Move(other.m_Capacity);
					}
					else
					{
						if (other.m_Size > m_Capacity)
						{
							ReAllocate(other.m_Size);
						}
						MemCpy(m_Data, other.m_Data, other.m_Size * sizeof(CharType));
						other.DeleteData();
					}
				}
				else
				{
					// We don't have any memory to free first
					MemCpy(m_SSO, other.m_Data, other.m_Size * sizeof(CharType));
					other.DeleteData();
				}
				other.m_Data = nullptr;
				other.m_Capacity = 0;
			}
			else
			{
				// There's nothing to delete because the string to be moved is using SSO, so no heap allocation

				if (IsHeapData())
				{
					if (other.m_Size > m_Capacity)
					{
						ReAllocate(other.m_Size);
					}
					MemCpy(m_Data, other.m_SSO, other.m_Size * sizeof(CharType));
				}
				else
				{
					MemCpy(m_SSO, other.m_SSO, other.m_Size * sizeof(CharType));
				}
			}

			other.m_Size = 1;
			other.m_SSO[0] = 0;
			return *this;
		}

		TString& operator=(const CharType* other)
		{
			if (m_Data)
			{
				DeleteData();
				m_Data = nullptr;
				m_Capacity = 0;
			}

			uint64 otherSize = (uint64)StrLength(other) + 1;
			m_Size = otherSize;

			if (IsHeapData())
			{
				if (otherSize > m_Capacity)
				{
					ReAllocate(otherSize);
				}
				MemCpy(m_Data, other, otherSize * sizeof(CharType));
			}
			else
			{
				MemCpy(m_SSO, other, otherSize * sizeof(CharType));
			}

			return *this;
		}

		bool operator==(const TStringView<CharType>& other) const
		{
			return Equals(other, ESearchCase::CaseSensitive);
		}

		bool operator!=(const TStringView<CharType>& other) const
		{
			return !(*this == other);
		}

		TString operator+(const TStringView<CharType>& substring) const
		{
			TString temp;
			temp.m_Size = m_Size + substring.Size() - 1;

			CharType* tempData = temp.m_SSO;
			CharType* thisData = Data();

			if (temp.IsHeapData())
			{
				temp.ReAllocate(temp.m_Size);
				tempData = temp.m_Data;
			}
			MemCpy(tempData, thisData, (m_Size - 1) * sizeof(CharType));
			MemCpy(tempData + (m_Size - 1), substring.Data(), substring.Size() * sizeof(CharType));

			return temp;
		}

		TString& operator+=(const TStringView<CharType>& substring)
		{
			Append(substring);
			return *this;
		}

	private:
		void DeleteData()
		{
			m_Allocator->Free(m_Data, m_Capacity * sizeof(CharType), EAllocatorHint::String);
		}

		void ReAllocate(uint64 newCapacity)
		{
			if (!m_Data)
			{
				m_Data = (CharType*)m_Allocator->Alloc((uint64)newCapacity * sizeof(CharType), EAllocatorHint::String);
			}
			else
			{
				m_Data = (CharType*)m_Allocator->Realloc(m_Data, (uint64)newCapacity * sizeof(CharType), EAllocatorHint::String);
			}
			m_Capacity = newCapacity;
		}

		void ReAllocateCopy(uint64 newCapacity)
		{
			if (!m_Data)
			{
				m_Data = (CharType*)m_Allocator->Alloc((uint64)newCapacity * sizeof(CharType), EAllocatorHint::String);
			}
			else
			{
				CharType* newBlock = (CharType*)m_Allocator->ExpandBlock(m_Data, (uint64)newCapacity * sizeof(CharType), EAllocatorHint::String);
				if (!newBlock)
				{
					newBlock = (CharType*)m_Allocator->Alloc((uint64)newCapacity * sizeof(CharType), EAllocatorHint::String);
					MemCpy(newBlock, m_Data, m_Size * sizeof(CharType));
					DeleteData();
				}
				m_Data = newBlock;
			}
			m_Capacity = newCapacity;
		}

	private:
		CharType* m_Data = nullptr;
		AllocatorType* m_Allocator = nullptr;

		uint64 m_Capacity = 0;
		uint64 m_Size = 0;

		CharType m_SSO[SSOBufferSize] = { 0 };

	private:
		friend class TString;
	};

	template<typename CharType>
	TStringView<CharType>::TStringView(const CharType* string)
	{
		m_Data = string;
		m_Size = StrLength(string) + 1;
	}

	template<typename CharType>
	TStringView<CharType>::TStringView(const CharType* string, uint64 stringSize)
	{
		m_Data = string;
		m_Size = stringSize;
	}
	
	template<typename CharType>
	template<uint64 SSOBufferSize, typename AllocatorType>
	TStringView<CharType>::TStringView(const TString<CharType, SSOBufferSize, AllocatorType>& string)
	{
		m_Data = string.Data();
		m_Size = string.Size();
	}

	// Typedefs

	////////////////////////
	/*    8-bit Strings   */
	////////////////////////

	using String      = TString<char8,  16,  HeapAllocator>;
									    
	using String8     = TString<char8,  8,   HeapAllocator>;
	using String16    = TString<char8,  16,  HeapAllocator>;
	using String32    = TString<char8,  32,  HeapAllocator>;
	using String48    = TString<char8,  48,  HeapAllocator>;
	using String64    = TString<char8,  64,  HeapAllocator>;
	using String96    = TString<char8,  96,  HeapAllocator>;
	using String128   = TString<char8,  128, HeapAllocator>;
	using String256   = TString<char8,  256, HeapAllocator>;

	using StringView  = TStringView<char8>;


	////////////////////////
	/*   16-bit Strings   */
	////////////////////////

	using WString     = TString<char16, 16,  HeapAllocator>;

	using WString8    = TString<char16, 8,   HeapAllocator>;
	using WString16   = TString<char16, 16,  HeapAllocator>;
	using WString32   = TString<char16, 32,  HeapAllocator>;
	using WString48   = TString<char16, 48,  HeapAllocator>;
	using WString64   = TString<char16, 64,  HeapAllocator>;
	using WString96   = TString<char16, 96,  HeapAllocator>;
	using WString128  = TString<char16, 128, HeapAllocator>;
	using WString256  = TString<char16, 256, HeapAllocator>;

	using WStringView = TStringView<char16>;

}