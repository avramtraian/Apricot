// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "Format.h"

#include "Apricot/Core/Memory/Memory.h"

namespace Apricot {
	
	template<typename T>
	APRICOT_API uint64 FormatType(const T& Value, TChar* Buffer, uint64 BufferSize)
	{
		AE_STATIC_ASSERT(false, "Specialize FormatType<T> in order to format this type!");
		return 0;
	}

	template<>
	APRICOT_API uint64 FormatType(const uint64& Value, TChar* Buffer, uint64 BufferSize)
	{
		if (Value == 0)
		{
			if (BufferSize >= 1)
			{
				Buffer[0] = '0';
				return 1;
			}
			return 0;
		}

		auto Temp = Value;
		uint8 Digits[255] = { 0 };
		uint8 DigitsCount = 0;
		while (Temp != 0)
		{
			Digits[DigitsCount] = (uint8)(Temp % 10);
			DigitsCount++;
			Temp /= 10;
		}

		for (int64 Index = DigitsCount - 1; Index >= 0; Index--)
		{
			if ((uint64)(DigitsCount - Index - 1) > BufferSize)
			{
				return BufferSize;
			}
			Buffer[DigitsCount - Index - 1] = '0' + Digits[Index];
		}

		return DigitsCount;
	}

	template<>
	APRICOT_API uint64 FormatType(const uint8& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<uint64>((uint64)Value, Buffer, BufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const uint16& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<uint64>((uint64)Value, Buffer, BufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const uint32& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<uint64>((uint64)Value, Buffer, BufferSize);
	}

	template<>
	APRICOT_API uint64 FormatType(const int64& Value, TChar* Buffer, uint64 BufferSize)
	{
		if (Value == 0)
		{
			if (BufferSize >= 1)
			{
				Buffer[0] = '0';
				return 1;
			}
			return 0;
		}

		auto Temp = Value;
		uint8 bIsNegative = Value < 0 ? 1 : 0;
		if (bIsNegative)
		{
			Temp *= -1;
			Buffer[0] = '-';
		}

		uint8 Digits[255] = { 0 };
		uint8 DigitsCount = 0;
		while (Temp != 0)
		{
			Digits[DigitsCount] = (uint8)(Temp % 10);
			DigitsCount++;
			Temp /= 10;
		}

		for (int64 Index = DigitsCount - 1; Index >= 0; Index--)
		{
			if ((uint64)(DigitsCount - Index - 1 + bIsNegative) > BufferSize)
			{
				return BufferSize;
			}
			Buffer[DigitsCount - Index - 1 + bIsNegative] = '0' + Digits[Index];
		}

		return DigitsCount + bIsNegative;
	}

	template<>
	APRICOT_API uint64 FormatType(const int8& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<int64>((int64)Value, Buffer, BufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const int16& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<int64>((int64)Value, Buffer, BufferSize);
	}
	
	template<>
	APRICOT_API uint64 FormatType(const int32& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<int64>((int64)Value, Buffer, BufferSize);
	}

	template<>
	APRICOT_API uint64 FormatType(const float64& Value, TChar* Buffer, uint64 BufferSize)
	{
		uint8 IntegralDigits[32] = { 0 };
		uint8 IntegralDigitsCount = 0;

		uint8 FractionalDigits[32] = { 0 };
		uint8 FractionalDigitsCount = 0;

		uint8 bIsNegative = 0;

		{
			int64 Temp = (int64)Value;
			if (-1 < Value && Value < 1)
			{
				IntegralDigits[0] = 0;
				IntegralDigitsCount = 1;
			}
			if (Value < 0)
			{
				Temp *= -1;
				bIsNegative = 1;
			}

			while (Temp != 0)
			{
				IntegralDigits[IntegralDigitsCount] = Temp % 10;
				IntegralDigitsCount++;
				Temp /= 10;
			}
		}

		{
			uint64 Offset = 10;
			float64 PositiveValue = bIsNegative ? -Value : Value;
			for (uint64 Index = 0; Index < AEC_FLOAT_LOG_PRECISION; Index++)
			{
				int64 Temp = (int64)(PositiveValue * (float64)Offset);
				FractionalDigits[FractionalDigitsCount] = Temp % 10;
				FractionalDigitsCount++;
				Offset *= 10;
			}
		}

		uint64 BufferOffset = 0;
		if (bIsNegative)
		{
			if (BufferOffset > BufferSize)
			{
				return BufferSize;
			}
			Buffer[BufferOffset] = '-';
			BufferOffset++;
		}

		for (int64 Index = IntegralDigitsCount - 1; Index >= 0; Index--)
		{
			if (BufferOffset > BufferSize)
			{
				return BufferSize;
			}
			Buffer[BufferOffset] = '0' + IntegralDigits[Index];
			BufferOffset++;
		}

		if (BufferOffset > BufferSize)
		{
			return BufferSize;
		}
		Buffer[BufferOffset] = '.';
		BufferOffset++;

		for (uint64 Index = 0; Index < FractionalDigitsCount; Index++)
		{
			if (BufferOffset > BufferSize)
			{
				return BufferSize;
			}
			Buffer[BufferOffset] = '0' + FractionalDigits[Index];
			BufferOffset++;
		}

		return BufferOffset;
	}

	template<>
	APRICOT_API uint64 FormatType(const float32& Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<float64>((float64)Value, Buffer, BufferSize);
	}

	template<>
	APRICOT_API uint64 FormatType(const bool8& Value, TChar* Buffer, uint64 BufferSize)
	{
		if (Value)
		{
			uint64 Size = sizeof("true") - 1;
			if (Size > BufferSize)
			{
				Size = BufferSize;
			}
			Memory_Copy(Buffer, "true", Size);
			return Size;
		}
		else
		{
			uint64 Size = sizeof("false") - 1;
			if (Size > BufferSize)
			{
				Size = BufferSize;
			}
			Memory_Copy(Buffer, "false", Size);
			return Size;
		}
	}

	template<>
	APRICOT_API uint64 FormatType(const TChar* Value, TChar* Buffer, uint64 BufferSize)
	{
		uint64 StringSize = Str_Length(Value);
		if (BufferSize < StringSize)
		{
			StringSize = BufferSize;
		}

		Memory_Copy(Buffer, Value, StringSize * sizeof(TChar));
		return StringSize;
	}

	template<>
	APRICOT_API uint64 FormatType(TChar* Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<const TChar>(Value, Buffer, BufferSize);
	}

	template<>
	APRICOT_API uint64 FormatType(const void* Value, TChar* Buffer, uint64 BufferSize)
	{
		return FormatType<uint64>((uint64)Value, Buffer, BufferSize);
	}

}