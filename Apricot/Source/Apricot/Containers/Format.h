// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "Apricot/Core/Base.h"
#include "Strings/String.h"

namespace Apricot {

	template<typename T>
	APRICOT_API uint64 FormatType(const T& Value, TChar* Buffer, uint64 BufferSize);

	template<typename T>
	APRICOT_API uint64 FormatType(T* Value, TChar* Buffer, uint64 BufferSize);

	inline void InternalFormat(const TChar* String, uint64 StringSize, TChar* Buffer, uint64 BufferSize)
	{
		uint64 Offset = 0;

		while (Offset < StringSize && Offset < BufferSize)
		{
			Buffer[Offset] = String[Offset];
			Offset++;
		}

		Buffer[BufferSize - 1] = 0;
	}

	template<typename T, typename... Args>
	void InternalFormat(const TChar* String, uint64 StringSize, TChar* Buffer, uint64 BufferSize, const T& Value, Args&&... args)
	{
		uint64 StringOffset = 0;
		uint64 BufferOffset = 0;

		bool8 bIsInsideCurlies = false;

		while (StringOffset < StringSize && BufferOffset < BufferSize)
		{
			if (String[StringOffset] == '{')
			{
				bIsInsideCurlies = true;
			}
			if (String[StringOffset] == '}')
			{
				BufferOffset += FormatType(Value, Buffer + BufferOffset, BufferSize - BufferOffset);
				InternalFormat(String + StringOffset + 1, StringSize - StringOffset, Buffer + BufferOffset, BufferSize - BufferOffset, std::forward<Args>(args)...);
				break;
			}
			else if (!bIsInsideCurlies)
			{
				Buffer[BufferOffset] = String[StringOffset];
				BufferOffset++;
			}
			StringOffset++;
		}
	}

	template<typename... Args>
	bool Format(TChar* Buffer, uint64 BufferSize, const TChar* String, Args&&... args)
	{
		uint64 StringSize = StrLength(String);
		if (StringSize == 1)
		{
			return false;
		}

		InternalFormat(String, StringSize + 1, Buffer, BufferSize, std::forward<Args>(args)...);
		return true;
	}

}