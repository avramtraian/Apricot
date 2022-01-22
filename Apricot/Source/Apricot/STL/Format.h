#pragma once

#include "Apricot/Core/Base.h"
#include "String.h"

namespace Apricot {

	template<typename T>
	APRICOT_API uint64 FormatType(const T& value, char8* buffer, uint64 bufferSize);

	template<typename T>
	APRICOT_API uint64 FormatType(T* value, char8* buffer, uint64 bufferSize);

	inline void Internal_Format(const char8* string, uint64 stringSize, char8* buffer, uint64 bufferSize)
	{
		uint64 offset = 0;

		while (offset < stringSize && offset < bufferSize)
		{
			buffer[offset] = string[offset];
			offset++;
		}

		buffer[bufferSize - 1] = 0;
	}

	template<typename T, typename... Args>
	void Internal_Format(const char8* string, uint64 stringSize, char8* buffer, uint64 bufferSize, const T& value, Args&&... args)
	{
		uint64 stringOffset = 0;
		uint64 bufferOffset = 0;

		bool8 bIsInsideCurlies = false;

		while (stringOffset < stringSize && bufferOffset < bufferSize)
		{
			if (string[stringOffset] == '{')
			{
				bIsInsideCurlies = true;
			}
			if (string[stringOffset] == '}')
			{
				bufferOffset += FormatType(value, buffer + bufferOffset, bufferSize - bufferOffset);
				Internal_Format(string + stringOffset + 1, stringSize - stringOffset, buffer + bufferOffset, bufferSize - bufferOffset, std::forward<Args>(args)...);
				break;
			}
			else if (!bIsInsideCurlies)
			{
				buffer[bufferOffset] = string[stringOffset];
				bufferOffset++;
			}
			stringOffset++;
		}
	}

	template<typename... Args>
	void Format(const char8* string, char8* buffer, uint64 bufferSize, Args&&... args)
	{
		Internal_Format(string, Str_Length(string) + 1, buffer, bufferSize, std::forward<Args>(args)...);
	}

}