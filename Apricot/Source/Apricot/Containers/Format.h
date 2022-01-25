// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "String.h"

namespace Apricot {

	template<typename T>
	APRICOT_API uint64 FormatType(const T& Value, char8* Buffer, uint64 BufferSize);

	template<typename T>
	APRICOT_API uint64 FormatType(T* Value, char8* Buffer, uint64 BufferSize);

	inline void Internal_Format(const char8* String, uint64 StringSize, char8* Buffer, uint64 BufferSize)
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
	void Internal_Format(const char8* String, uint64 StringSize, char8* Buffer, uint64 BufferSize, const T& Value, Args&&... args)
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
				Internal_Format(String + StringOffset + 1, StringSize - StringOffset, Buffer + BufferOffset, BufferSize - BufferOffset, std::forward<Args>(args)...);
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
	void Format(char8* Buffer, uint64 BufferSize, const char8* String, Args&&... args)
	{
		Internal_Format(String, Str_Length(String) + 1, Buffer, BufferSize, std::forward<Args>(args)...);
	}

}