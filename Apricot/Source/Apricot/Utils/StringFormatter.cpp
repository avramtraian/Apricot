#include "aepch.h"
#include "StringFormatter.h"

namespace Apricot {

	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q& value)
	{
		STATIC_ASSERT(false, "Implement THFormatter<T>& operator<<(THFormatter<T>&, const Q&) in order to format this type!");
		return formatter;
	}

	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q* value)
	{
		STATIC_ASSERT(false, "Implement THFormatter<T>& operator<<(THFormatter<T>&, const Q*) in order to format this type!");
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const uint64& value)
	{
		if (value == 0)
		{
			formatter.Data = (char8*)formatter.Allocator->Allocate(sizeof(char8));
			formatter.Data[0] = '0';
			formatter.Size = sizeof(char8);
			return formatter;
		}

		uint64 requiredAllocationSize = 0;
		auto tempValue = value;

		uint8 digits = 0;
		while (tempValue != 0)
		{
			digits++;
			requiredAllocationSize += sizeof(char8);
			tempValue /= 10;
		}

		char8* allocation = (char8*)formatter.Allocator->Allocate(requiredAllocationSize);
		formatter.Data = allocation;
		formatter.Size = requiredAllocationSize;

		tempValue = value;
		uint64 offset = 0;
		while (tempValue != 0)
		{
			allocation[digits - offset - 1] = '0' + tempValue % 10;
			tempValue /= 10;
			offset++;
		}

		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const int64& value)
	{
		if (value == 0)
		{
			formatter.Data = (char8*)formatter.Allocator->Allocate(sizeof(char8));
			formatter.Data[0] = '0';
			formatter.Size = sizeof(char8);
			return formatter;
		}

		uint64 requiredAllocationSize = 0;
		uint8 sign = 0;
		auto tempValue = value;
		if (value < 0)
		{
			sign = 1;
			tempValue *= -1;
			requiredAllocationSize += sizeof(char8);
		}

		uint8 digits = 0;
		while (tempValue != 0)
		{
			digits++;
			requiredAllocationSize += sizeof(char8);
			tempValue /= 10;
		}

		char8* allocation = (char8*)formatter.Allocator->Allocate(requiredAllocationSize);
		formatter.Data = allocation;
		formatter.Size = requiredAllocationSize / sizeof(char8);

		tempValue = value;
		if (value < 0)
		{
			tempValue *= -1;
			allocation[0] = '-';
		}

		uint64 offset = 0;
		while (tempValue != 0)
		{
			allocation[digits + sign - offset - 1] = '0' + tempValue % 10;
			tempValue /= 10;
			offset++;
		}

		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const float64& value)
	{
		int64 valueInt = (int64)value;

		// Digits from right to left
		uint8 integralDigits[32] = {};
		uint8 integralDigitsCount = 0;

		// Digits from left to right
		uint8 fractionalDigits[32] = {};
		uint8 fractionalDigitsCount = 0;

		uint8 bIsNegative = false;

		if (valueInt == 0)
		{
			integralDigitsCount = 1;
			integralDigits[0] = 0;
		}
		else if (valueInt < 0)
		{
			valueInt = -valueInt;
			bIsNegative = true;
		}

		while (valueInt != 0)
		{
			integralDigits[integralDigitsCount] = valueInt % 10;
			integralDigitsCount++;
			valueInt /= 10;
		}

		uint64 offset = 10;
		float64 positiveValue = bIsNegative ? -value : value;
		for (uint64 index = 0; index < AEC_FLOAT_LOG_PRECISION; index++)
		{
			int64 temp = (int64)(positiveValue * (float64)offset);
			fractionalDigits[fractionalDigitsCount] = temp % 10;
			fractionalDigitsCount++;
			offset *= 10;
		}

		char8* allocation = formatter.Allocate(((uint64)bIsNegative + integralDigitsCount + 1 + fractionalDigitsCount) * sizeof(char8));
		offset = 0;
		if (bIsNegative)
		{
			allocation[offset] = '-';
			offset++;
		}

		for (uint64 index = integralDigitsCount; index > 0; index--)
		{
			allocation[offset] = '0' + integralDigits[index - 1];
			offset++;
		}

		allocation[offset] = '.';
		offset++;

		for (uint64 index = 0; index < fractionalDigitsCount; index++)
		{
			allocation[offset] = '0' + fractionalDigits[index];
			offset++;
		}

		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const uint8& value)
	{
		formatter << (uint64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const uint16& value)
	{
		formatter << (uint64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const uint32& value)
	{
		formatter << (uint64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const int8& value)
	{
		formatter << (int64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const int16& value)
	{
		formatter << (int64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const int32& value)
	{
		formatter << (int64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const float32& value)
	{
		formatter << (float64)value;
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const char8* value)
	{
		formatter.Size = strlen(value);
		formatter.Data = (char8*)formatter.Allocator->Allocate(formatter.Size * sizeof(char8));
		Memory::Copy(formatter.Data, value, formatter.Size * sizeof(char8));

		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const String& value)
	{
		char8* allocation = formatter.Allocate((value.Size() - 1) * sizeof(char8));
		Memory::Copy(allocation, value.Data(), formatter.Size * sizeof(char8));
		return formatter;
	}

	template<>
	APRICOT_API THFormatter<char8>& operator<<(THFormatter<char8>& formatter, const StringView& value)
	{
		char8* allocation = formatter.Allocate((value.Size() - 1) * sizeof(char8));
		Memory::Copy(allocation, value.String(), formatter.Size * sizeof(char8));
		return formatter;
	}

}