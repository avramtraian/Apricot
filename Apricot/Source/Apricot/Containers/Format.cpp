// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Format.h"

#include "Apricot/Core/Memory.h"

namespace Apricot {
	
	template<typename T>
	APRICOT_API uint64 FormatType(const T& value, char8* buffer, uint64 bufferSize)
	{
		AE_STATIC_ASSERT(false, "Specialize FormatType<T> in order to format this type!");
		return 0;
	}



	template<>
	APRICOT_API uint64 FormatType(const uint64& value, char8* buffer, uint64 bufferSize)
	{
		if (value == 0)
		{
			if (bufferSize >= 1)
			{
				buffer[0] = '0';
				return 1;
			}
			return 0;
		}

		auto temp = value;
		uint8 digits[255] = { 0 };
		uint8 digitsCount = 0;
		while (temp != 0)
		{
			digits[digitsCount] = (uint8)(temp % 10);
			digitsCount++;
			temp /= 10;
		}

		for (int64 index = digitsCount - 1; index >= 0; index--)
		{
			if ((uint64)(digitsCount - index - 1) > bufferSize)
			{
				return bufferSize;
			}
			buffer[digitsCount - index - 1] = '0' + digits[index];
		}

		return digitsCount;
	}

	template<>
	APRICOT_API uint64 FormatType(const uint8& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<uint64>((uint64)value, buffer, bufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const uint16& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<uint64>((uint64)value, buffer, bufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const uint32& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<uint64>((uint64)value, buffer, bufferSize);
	}



	template<>
	APRICOT_API uint64 FormatType(const int64& value, char8* buffer, uint64 bufferSize)
	{
		if (value == 0)
		{
			if (bufferSize >= 1)
			{
				buffer[0] = '0';
				return 1;
			}
			return 0;
		}

		auto temp = value;
		uint8 bIsNegative = value < 0 ? 1 : 0;
		if (bIsNegative)
		{
			temp *= -1;
			buffer[0] = '-';
		}

		uint8 digits[255] = { 0 };
		uint8 digitsCount = 0;
		while (temp != 0)
		{
			digits[digitsCount] = (uint8)(temp % 10);
			digitsCount++;
			temp /= 10;
		}

		for (int64 index = digitsCount - 1; index >= 0; index--)
		{
			if ((uint64)(digitsCount - index - 1 + bIsNegative) > bufferSize)
			{
				return bufferSize;
			}
			buffer[digitsCount - index - 1 + bIsNegative] = '0' + digits[index];
		}

		return digitsCount + bIsNegative;
	}

	template<>
	APRICOT_API uint64 FormatType(const int8& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<int64>((int64)value, buffer, bufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const int16& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<int64>((int64)value, buffer, bufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const int32& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<int64>((int64)value, buffer, bufferSize);
	}



	template<>
	APRICOT_API uint64 FormatType(const float64& value, char8* buffer, uint64 bufferSize)
	{
		uint8 integralDigits[32] = { 0 };
		uint8 integralDigitsCount = 0;

		uint8 fractionalDigits[32] = { 0 };
		uint8 fractionalDigitsCount = 0;

		uint8 bIsNegative = 0;

		{
			int64 temp = (int64)value;
			if (-1 < value && value < 1)
			{
				integralDigits[0] = 0;
				integralDigitsCount = 1;
			}
			if (value < 0)
			{
				temp *= -1;
				bIsNegative = 1;
			}

			while (temp != 0)
			{
				integralDigits[integralDigitsCount] = temp % 10;
				integralDigitsCount++;
				temp /= 10;
			}
		}

		{
			uint64 offset = 10;
			float64 positiveValue = bIsNegative ? -value : value;
			for (uint64 index = 0; index < AEC_FLOAT_LOG_PRECISION; index++)
			{
				int64 temp = (int64)(positiveValue * (float64)offset);
				fractionalDigits[fractionalDigitsCount] = temp % 10;
				fractionalDigitsCount++;
				offset *= 10;
			}
		}

		uint64 bufferOffset = 0;
		if (bIsNegative)
		{
			if (bufferOffset > bufferSize)
			{
				return bufferSize;
			}
			buffer[bufferOffset] = '-';
			bufferOffset++;
		}

		for (int64 index = integralDigitsCount - 1; index >= 0; index--)
		{
			if (bufferOffset > bufferSize)
			{
				return bufferSize;
			}
			buffer[bufferOffset] = '0' + integralDigits[index];
			bufferOffset++;
		}

		if (bufferOffset > bufferSize)
		{
			return bufferSize;
		}
		buffer[bufferOffset] = '.';
		bufferOffset++;

		for (uint64 index = 0; index < fractionalDigitsCount; index++)
		{
			if (bufferOffset > bufferSize)
			{
				return bufferSize;
			}
			buffer[bufferOffset] = '0' + fractionalDigits[index];
			bufferOffset++;
		}

		return bufferOffset;
	}

	template<>
	APRICOT_API uint64 FormatType(const float32& value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<float64>((float64)value, buffer, bufferSize);
	}



	template<>
	APRICOT_API uint64 FormatType(const char8* value, char8* buffer, uint64 bufferSize)
	{
		uint64 stringSize = Str_Length(value);
		if (bufferSize < stringSize)
		{
			stringSize = bufferSize;
		}

		Memory_Copy(buffer, value, stringSize * sizeof(char8));
		return stringSize;
	}

	template<>
	APRICOT_API uint64 FormatType(char8* value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<const char8>(value, buffer, bufferSize);
	}

	template<>
	APRICOT_API uint64 FormatType(const void* value, char8* buffer, uint64 bufferSize)
	{
		return FormatType<uint64>((uint64)value, buffer, bufferSize);
	}

}