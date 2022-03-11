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
	* 
	*/
	enum class ESearchDirection : uint8
	{
		None = 0,

		FromEnd,
		FromBeginning,

		MaxEnumValue,
	};

	/**
	* 
	*/
	enum class ESearchCase : uint8
	{
		None = 0,

		Ignore,
		CaseSensitive,

		MaxEnumValue,
	};

	template<typename CharType>
	inline CharType ToLowerCase(CharType character)
	{
		if ('A' <= character && character <= 'Z')
		{
			return character + ('a' - 'A');
		}
		return character;
	}

	// Forward declarations
	template<typename CharType, uint64 SSOBufferSize, typename AllocatorType>
	class TString;

	/**
	* C++ Core Engine Architecture
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

	public:
		
	private:
		const CharType* m_Data = nullptr;
		uint64 m_Size = 0;
	};

	/**
	* C++ Core Engine Architecture
	*/
	template<typename CharType, uint64 SSOBufferSize, typename AllocatorType = HeapAllocator>
	class TString
	{
	public:
		static constexpr uint64 InvalidPos = static_cast<uint64>(-1);

	public:
		TString(AllocatorType* allocator = AllocatorType::GetDefault())
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

		void SetCapacity(uint64 newCapacity)
		{
			if (newCapacity > SSOBufferSize)
			{
				
			}
			else
			{
				
			}
		}

	public:
		NODISCARD uint64 Find(const TStringView<CharType>& substring, ESearchCase searchCase = ESearchCase::CaseSensitive, ESearchDirection searchDirection = ESearchDirection::FromBeginning, uint64 searchOffsetBeginning = 0, uint64 searchOffsetEnd = 0) const
		{
			const CharType* data = Data();

			if (searchDirection == ESearchDirection::FromBeginning)
			{
				int64 maxStringIndex = (int64)m_Size - (int64)substring.Size() - (int64)searchOffsetEnd;
				for (int64 stringIndex = searchOffsetBeginning; stringIndex <= maxStringIndex; stringIndex++)
				{
					bool bFound = true;
					for (uint64 substringIndex = 0; substringIndex < substring.Size() - 1; substringIndex++)
					{
						bool bAreMatching = true;
						switch (searchCase)
						{
							case ESearchCase::Ignore:
								bAreMatching = ToLowerCase(data[stringIndex + substringIndex]) == ToLowerCase(substring.Data()[substringIndex]);
								break;
							case ESearchCase::CaseSensitive:
								bAreMatching = (data[stringIndex + substringIndex] == substring.Data()[substringIndex]);
								break;
							default:
								AE_CORE_ASSERT_NO_ENTRY();
								break;
						}
						if (!bAreMatching)
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
			else if (searchDirection == ESearchDirection::FromEnd)
			{
				for (int64 stringIndex = m_Size - substring.Size() - searchOffsetEnd; stringIndex >= (int64)searchOffsetBeginning; stringIndex--)
				{
					bool bFound = true;
					for (uint64 substringIndex = 0; substringIndex < substring.Size() - 1; substringIndex++)
					{
						bool bAreMatching = true;
						switch (searchCase)
						{
							case ESearchCase::Ignore:
								bAreMatching = ToLowerCase(data[stringIndex + substringIndex]) == ToLowerCase(substring.Data()[substringIndex]);
								break;
							case ESearchCase::CaseSensitive:
								bAreMatching = (data[stringIndex + substringIndex] == substring.Data()[substringIndex]);
								break;
							default:
								AE_CORE_ASSERT_NO_ENTRY();
								break;
						}
						if (!bAreMatching)
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

		template<typename NewCharType, typename NewAllocatorType = HeapAllocator, uint64 NewSSOBufferSize = SSOBufferSize>
		NODISCARD TString<NewCharType, NewSSOBufferSize, NewAllocatorType> As(NewAllocatorType* newAllocator = AllocatorType::GetDefault()) const
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

	using String = TString<TChar, 16, HeapAllocator>;

	using String8   = TString<TChar, 8,   HeapAllocator>;
	using String16  = TString<TChar, 16,  HeapAllocator>;
	using String32  = TString<TChar, 32,  HeapAllocator>;
	using String48  = TString<TChar, 48,  HeapAllocator>;
	using String64  = TString<TChar, 64,  HeapAllocator>;
	using String96  = TString<TChar, 96,  HeapAllocator>;
	using String128 = TString<TChar, 128, HeapAllocator>;
	using String256 = TString<TChar, 256, HeapAllocator>;

	using StringView = TStringView<TChar>;

}