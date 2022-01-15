// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Core/Allocators.h"

namespace Apricot {

	/*
	* The class in charge of formatting any type.
	* Has strong relationship with the 'THFormatter<T>& operator<<(THFormatter<T>&, const Q&)' function.
	*
	* @param T The type of string's characters (char8 or char16).
	*/
	template<typename T>
	struct THFormatter
	{
	public:
		/*
		* Internal usage only.
		* You should never instantiate a THFormatter on your own.
		*/
		THFormatter(Memory::LinearAllocator* allocator)
			: Allocator(allocator) {}

		/*
		* A thin wrapper function for the allocator's interface.
		* It sets the formatter's Size to the requested memory block size.
		* It's recommended to use this function.
		*
		* @param size The size (in bytes) of the requested allocation.
		* @returns Pointer to the allocated memory. It is the same as 'Data'.
		*/
		T* Allocate(uint64 size)
		{
			Data = (T*)Allocator->Allocate(size);
			Size = size;
			return Data;
		}

	public:
		/*
		* The allocator used to allocate memory for the string.
		*/
		Memory::LinearAllocator* Allocator = nullptr;

		/*
		* The address in the allocator where the string is.
		*/
		T* Data = nullptr;

		/*
		* The size (in bytes) of the string.
		* Excludes the null-terminating character.
		*/
		uint64 Size = 0;
	};

	/*
	* Implement a template specialization of this function in order to format any type.
	* Remember that the size SHOULD NEVER include the null-terminating character.
	* Also, there is no need for the null-terminating character to exist.
	* We are more space efficient without it.
	*
	* @param typename T The type of characters of the returned string (char8 or char16).
	* @param typename Q The type of the class you want to format.
	*
	* @param formatter The struct to fill with the string buffer.
	* @param value The value to be formatted.
	*/
	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q& value);
	template<typename T, typename Q>
	APRICOT_API THFormatter<T>& operator<<(THFormatter<T>& formatter, const Q* value);


	/*
	* Helper class used for formatting C-Style strings.
	*/
	class APRICOT_API RawStringFormatter
	{
	public:
		/*
		* Formats a string. Doesn't allocate any memory on the heap (works only with allocators).
		*
		* @param typename T Type of string (char8 or char16).
		* @param typename... Args Argument pack.
		*
		* @param allocator Allocator used for allocating memory.
		* @param message C-Style string with the (unformatted) message.
		* @param args Argument pack.
		*
		* @returns The formatted string.
		*/
		template<typename T, typename... Args>
		static const T* Format(Memory::LinearAllocator& allocator, const T* message, Args&&... args)
		{
			THFormatter<char8>* formatters = (THFormatter<char8>*)allocator.Allocate(AEC_CSTR_MAX_FORMATTERS * sizeof(THFormatter<char8>));
			for (uint64 index = 0; index < AEC_CSTR_MAX_FORMATTERS; index++)
			{
				Memory::PlacementNew<THFormatter<char8>>(formatters + index, &allocator);
			}

			uint64 usedFormattersCount = 0;
			return FormatComplex(allocator, formatters, AEC_CSTR_MAX_FORMATTERS, usedFormattersCount, message, std::forward<Args>(args)...);
		}

		/*
		* Formats a string. Doesn't allocate any memory on the heap (works only with allocators).
		* 
		* @param typename T Type of string (char8 or char16).
		* @param typename... Args Argument pack.
		* 
		* @param allocator Allocator used for allocating memory.
		* @param formatters Array of THFormatter<T>. Used for formatting the arguments.
		* @param formattersCount Count of formatters.
		* @param outFormattersCount Out variable, represents the number of actually used formatters.
		* @param message C-Style string with the (unformatted) message.
		* @param args Argument pack.
		* 
		* @returns The formatted string.
		*/
		template<typename T, typename... Args>
		static const T* FormatComplex(Memory::LinearAllocator& allocator, THFormatter<T>* formatters, uint64 formattersCount, uint64& outFormattersCount, const T* message, Args&&... args)
		{
			// Fill the formatters.
			outFormattersCount = 0;
			FormatArguments(formatters, formattersCount, outFormattersCount, std::forward<Args>(args)...);

			// Save the beginning of the formatted string.
			T* formatted = (T*)allocator.GetCurrentMark();

			// Iterate over all characters of the message.
			int64 index = -1;
			uint64 argumentIndex = 0;
			bool bInsideArg = false;
			do
			{
				index++;
				switch (message[index])
				{
					case '{':
					{
						if (bInsideArg)
						{
							// TODO: Warn
						}
						bInsideArg = true;
						break;
					}
					case '}':
					{
						if (!bInsideArg)
						{
							// TODO: Warn
						}
						bInsideArg = false;


						if (argumentIndex < formattersCount)
						{
							T* buffer = (T*)allocator.Allocate(formatters[argumentIndex].Size);
							Memory::Copy(buffer, formatters[argumentIndex].Data, formatters[argumentIndex].Size);
							argumentIndex++;
						}
						else
						{
							T* buffer = (T*)allocator.Allocate(4 * sizeof(T));
							Memory::Copy(buffer, "????", 4 * sizeof(T));
						}

						break;
					}
					default:
					{
						if (!bInsideArg)
						{
							T* buffer = (T*)allocator.Allocate(sizeof(T));
							*buffer = message[index];
						}
						break;
					}
				}
			} while (message[index] != 0);

			return formatted;
		}

	private:
		/*
		* Internal usage only.
		*/
		template<typename T, typename Q, typename... Args>
		static void FormatArguments(THFormatter<T>* formatters, uint64 formattersCount, uint64& outFormattersCount, const Q& argument, Args&&... args)
		{
			if (outFormattersCount >= formattersCount)
			{
				return;
			}

			formatters[outFormattersCount] << argument;
			outFormattersCount++;

			FormatArguments(formatters, formattersCount, outFormattersCount, std::forward<Args>(args)...);
		}
		
		/*
		* Internal usage only.
		* Base case for iterating over argument pack.
		*/
		template<typename T, typename... Args>
		static void FormatArguments(THFormatter<T>* formatters, uint64 formattersCount, uint64& outFormattersCount)
		{
		}
	};

}