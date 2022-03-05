// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#include "aepch.h"
#include "Hash.h"

namespace Apricot {
	
	template<>
	APRICOT_API uint64 GetTypeHash(const int8& Object)
	{
		return *(uint64*)(&Object) + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const int16& Object)
	{
		return *(uint64*)(&Object) + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const int32& Object)
	{
		return *(uint64*)(&Object) + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const int64& Object)
	{
		return *(uint64*)(&Object);
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const uint8& Object)
	{
		return (uint64)Object + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const uint16& Object)
	{
		return (uint64)Object + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const uint32& Object)
	{
		return (uint64)Object + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const uint64& Object)
	{
		return Object;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const char8& Object)
	{
		return (uint64)Object + 107u;
	}

	template<>
	APRICOT_API uint64 GetTypeHash(const char16& Object)
	{
		return (uint64)Object + 107u;
	}

}