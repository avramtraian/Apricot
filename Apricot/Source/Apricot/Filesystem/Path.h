// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/DataStructures/String.h"

namespace Apricot { namespace Filesystem {
	
	/*
	* Internal usage only.
	*/
	class PathAlias
	{
		char16* Data = nullptr;
		uint64 Capacity;
		uint64 Size = 0;
	};

	/*
	* Internal usage only.
	*/
	template<uint64 S>
	class TStackPathAlias
	{
		char16 Data[S] = { 0 };
		uint64 Size = 0;
	};

	class Path;

	template<uint64 S>
	class TStackPath;
	
	/*
	* String implementation. It is using char16 type.
	* Allocates all memory on the heap.
	*/
	class APRICOT_API Path
	{
	public:
		Path()
		{
			ReAllocate(1);
			m_Size = 1;
			m_Data[0] = 0;
		}

		Path(const Path& other)
		{
			ReAllocate(other.m_Size);
			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(char16));
			m_Size = other.m_Size;
		}

		Path(Path&& other) noexcept
		{
			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
		}

		Path(const char8* path)
		{
			m_Size = CStrLength(path) + 1;
			ReAllocate(m_Size);

			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)(path[index]);
			}
		}

		Path(const char16* path)
		{
			m_Size = CStrLength(path) + 1;
			ReAllocate(m_Size);
			Memory::Copy(m_Data, path, m_Size * sizeof(char16));
		}

		template<uint64 S>
		Path(const TStackPath<S>& path)
		{
			TStackPathAlias<S>* alias = (TStackPathAlias<S>*)(&path);

			m_Size = alias->Size;
			ReAllocate(m_Size);
			Memory::Copy(m_Data, alias->Data, m_Size * sizeof(char16));
		}

		template<typename T>
		Path(const TString<T>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
		}

		template<typename T, uint64 S>
		Path(const TStackString<T, S>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
		}

		template<typename T>
		Path(const TStringView<T>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
		}

		Path(uint64 capacity)
		{
			ReAllocate(capacity);
			m_Data[0] = 0;
			m_Size = 1;
		}

		~Path()
		{
			Memory::Free(m_Data, m_Capacity * sizeof(char16), Memory::AllocTag::Filesystem);
		}

	public:
		Path& operator=(const Path& other)
		{
			if (other.m_Size > m_Capacity)
			{
				ReAllocate(other.m_Size);
			}
			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(char16));
			m_Size = other.m_Size;
			return *this;
		}

		Path& operator=(Path&& other) noexcept
		{
			Memory::Free(m_Data, m_Capacity * sizeof(char16), Memory::AllocTag::Filesystem);

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;

			other.m_Data = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;

			return *this;
		}

		Path& operator=(const char8* path)
		{
			m_Size = CStrLength(path) + 1;
			if (m_Size > m_Capacity)
			{
				ReAllocate(m_Size);
			}

			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)(path[index]);
			}

			return *this;
		}

		Path& operator=(const char16* path)
		{
			m_Size = CStrLength(path) + 1;
			if (m_Size > m_Capacity)
			{
				ReAllocate(m_Size);
			}
			Memory::Copy(m_Data, path, m_Size * sizeof(char16));
			return *this;
		}

		template<uint64 S>
		Path& operator=(const TStackPath<S>& path)
		{
			TStackPathAlias<S>* alias = (TStackPathAlias<S>*)(&path);

			m_Size = alias->Size;
			if (m_Size > m_Capacity)
			{
				ReAllocate(m_Size);
			}
			Memory::Copy(m_Data, alias->Data, m_Size * sizeof(char16));
			return *this;
		}

		template<typename T>
		Path& operator=(const TString<T>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
			return *this;
		}

		template<typename T, uint64 S>
		Path& operator=(const TStackString<T, S>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
			return *this;
		}

		template<typename T>
		Path& operator=(const TStringView<T>& path)
		{
			m_Size = path.Size();
			ReAllocate(m_Size);
			for (uint64 index = 0; index < m_Size; index++)
			{
				m_Data[index] = (char16)path[index];
			}
			return *this;
		}

		const char16& operator[](uint64 index) const
		{
			return m_Data[index];
		}

		char16& operator[](uint64 index)
		{
			return m_Data[index];
		}

	public:
		char16* Data() const { return m_Data; }
		uint64 Capacity() const { return m_Capacity; }
		uint64 Size() const { return m_Size; }

		const char16* c_str() const { return m_Data; }

		void Reserve(uint64 newCapacity)
		{
			ReAllocateCopy(newCapacity);
		}

		void Resize(uint64 newSize)
		{
			if (newSize > m_Capacity)
			{
				ReAllocateCopy(newSize);
			}
			m_Size = newSize;
			m_Data[m_Size - 1] = 0;
		}

		void Clear()
		{
			m_Size = 1;
			m_Data[0] = 0;
		}

		void Tidy()
		{
			ReAllocateCopy(m_Size);
		}

		void TidyClear()
		{
			ReAllocate(1);
			Clear();
		}

		Path Parent() const
		{
			uint64 offset = m_Size;
			for (int64 index = m_Size - 1; index >= 0; index--)
			{
				if (m_Data[index] == '/')
				{
					offset--;
					break;
				}
				offset--;
			}

			Path parent(offset + 1);
			parent.Resize(parent.Capacity());

			Memory::Copy(parent.m_Data, m_Data, offset * sizeof(char16));
			parent.m_Data[parent.m_Size - 1] = 0;

			return parent;
		}

		Path File() const
		{
			uint64 fileOffset = m_Size;

			for (int64 index = m_Size - 1; index >= 0; index--)
			{
				if (m_Data[index] == '/')
				{
					break;
				}
				fileOffset--;
			}

			Path file(m_Size - fileOffset);
			file.Resize(file.Capacity());
			Memory::Copy(file.Data(), m_Data + fileOffset, (m_Size - fileOffset) * sizeof(char16));
			return file;
		}

		Path Name() const
		{
			uint64 fileOffset = m_Size;
			uint64 extensionSize = 0;
			bool bFoundExtension = false;

			for (int64 index = m_Size - 1; index >= 0; index--)
			{
				if (m_Data[index] == '/')
				{
					break;
				}
				else if (m_Data[index] == '.' && !bFoundExtension)
				{
					bFoundExtension = true;
				}

				fileOffset--;
				if (!bFoundExtension)
				{
					extensionSize++;
				}
			}

			Path name(m_Size - fileOffset - extensionSize);
			name.Resize(name.Capacity());
			Memory::Copy(name.Data(), m_Data + fileOffset, (m_Size - fileOffset - extensionSize) * sizeof(char16));
			name[name.Size() - 1] = 0;
			return name;
		}

		Path Extension() const
		{
			uint64 extensionOffset = m_Size;
			for (int64 index = m_Size - 1; index >= 0; index--)
			{
				if (m_Data[index] == '.')
				{
					extensionOffset--;
					break;
				}
				extensionOffset--;
			}

			Path extension(m_Size - extensionOffset);
			extension.Resize(extension.Capacity());
			Memory::Copy(extension.Data(), m_Data + extensionOffset, (m_Size - extensionOffset) * sizeof(char16));

			return extension;
		}

	private:
		void ReAllocate(uint64 newCapacity)
		{
			char16* newBlock = (char16*)Memory::Allocate(newCapacity * sizeof(char16), Memory::AllocTag::Filesystem);
			Memory::Free(m_Data, m_Capacity * sizeof(char16), Memory::AllocTag::Filesystem);

			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

		void ReAllocateCopy(uint64 newCapacity)
		{
			char16* newBlock = (char16*)Memory::Allocate(newCapacity * sizeof(char16), Memory::AllocTag::Filesystem);

			if (m_Size > newCapacity)
			{
				m_Size = newCapacity;
			}

			Memory::Copy(newBlock, m_Data, m_Size * sizeof(char16));
			newBlock[newCapacity - 1] = 0;
			Memory::Free(m_Data, m_Capacity * sizeof(char16), Memory::AllocTag::Filesystem);

			m_Data = newBlock;
			m_Capacity = newCapacity;
		}

	private:
		/*
		* 
		*/
		char16* m_Data = nullptr;

		/*
		*
		*/
		uint64 m_Capacity = 0;

		/*
		*
		*/
		uint64 m_Size = 0;
	};

	/*
	* String implementation. It is using char16 type.
	* It allocates all memory on the stack.
	* 
	* Currently, it is recommended to use this version of path as much as possible, due to less dynamic memory allocations.
	*/
	template<uint64 S>
	class TStackPath
	{
	public:
		TStackPath()
		{
			m_Data[0] = 0;
			m_Size = 1;
		}

		template<uint64 R>
		TStackPath(const TStackPath<R>& other)
		{
			AE_STATIC_ASSERT(R <= S, "Possible TStackPath buffer overflow!");

			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(char16));
			m_Size = other.m_Size;
		}

		TStackPath(const char16* other)
		{
			m_Size = CStrLength(other) + 1;
			Memory::Copy(m_Data, other, m_Size * sizeof(char16));
		}

	public:
		template<uint64 R>
		TStackPath<R>& operator=(const TStackPath<R>& other)
		{
			AE_STATIC_ASSERT(R <= S, "Possible TStackPath buffer overflow!");

			Memory::Copy(m_Data, other.m_Data, other.m_Size * sizeof(char16));
			m_Size = other.m_Size;

			return *this;
		}

		TStackPath<S> operator+(const char16* other) const
		{
			return Append_Impl<S>(other, CStrLength(other) + 1);
		}

	public:
		char16* Data() const { return const_cast<char16*>(&m_Data[0]); }
		uint64 MaxSize() const { return S; }
		uint64 Size() const { return m_Size; }

	private:
		template<uint64 R>
		TStackPath<R> Append_Impl(const char16* other, uint64 otherSize) const
		{
			AE_STATIC_ASSERT(S >= R, "TStackPath buffer overflow!");

			TStackPath<R> newString;
			newString.m_Size = m_Size + otherSize - 1;
			Memory::Copy(newString.m_Data, m_Data, (m_Size - 1) * sizeof(char16));

			uint8 offset = 0;
			if (m_Data[m_Size - 2] != '/' && other[0] != '/')
			{
				newString.m_Data[m_Size - 1] = '/';
				offset = 1;
			}

			Memory::Copy(newString.m_Data + m_Size - 1 + offset, other, otherSize * sizeof(char16));

			return newString;
		}

	public:
		/*
		*
		*/
		char16 m_Data[S] = { 0 };

		/*
		*
		*/
		uint64 m_Size = 0;
	};

	using Path8 = TStackPath<8>;
	using Path16 = TStackPath<16>;
	using Path32 = TStackPath<32>;
	using Path48 = TStackPath<48>;
	using Path64 = TStackPath<64>;
	using Path96 = TStackPath<96>;
	using Path128 = TStackPath<128>;
	using Path256 = TStackPath<256>;

} }