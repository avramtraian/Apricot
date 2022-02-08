// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/BaseTypes.h"

#include <new>

#define AE_ARRAY_LEN(Array)       (sizeof(Array) / sizeof(Array[0]))

#define AE_MEMZERO_STRUCT(Struct) { ::Apricot::MemZero( &Struct, sizeof(  Struct )); }
#define AE_MEMZERO_ARRAY(Array)   { ::Apricot::MemZero(  Array,  sizeof(  Array  )); }
#define AE_MEMZERO_PTR(Ptr)       { ::Apricot::MemZero(  Ptr,    sizeof( *Ptr    )); }

#define AE_INVALID_MEMSIZE (static_cast<uint64>(-1))

#define AE_MEMPRF AE_FILE, AE_FUNCTION, AE_LINE

namespace Apricot {

	/**
	* 
	*/
	APRICOT_API void ApricotMemoryInit();
	APRICOT_API void ApricotMemoryDestroy();

	/**
	* 
	*/
	class APlatformMemory
	{
	public:
		static void Init();
		static void Destroy();

		static void* Malloc(uint64 Size, uint64 Alignment);
		static void Free(void* MemoryBlock, uint64 Size);

		static void MemCpy(void* Destination, const void* Source, uint64 SizeBytes);
		static void MemSet(void* Destination, int32 Value, uint64 SizeBytes);
		static void MemZero(void* Destination, uint64 SizeBytes);

		static uint64 GetAllocationSize(void* Allocation);
	};

	/**
	* 
	*/
	class AMalloc
	{
	public:
		AMalloc();
		virtual ~AMalloc();

		APRICOT_API virtual void* Alloc(uint64 Size);
		APRICOT_API virtual void Free(void* Allocation);

		APRICOT_API virtual void* Alloc(uint64 Size, const TChar* File, const TChar* Func, uint64 Line);
		APRICOT_API virtual void Free(void* Allocation, const TChar* File, const TChar* Func, uint64 Line);
	};

	APRICOT_API extern AMalloc* GMalloc;

	APRICOT_API void MemCpy(const void* Source, void* Destination, uint64 SizeBytes);
	APRICOT_API void MemSet(void* Destination, int32 Value, uint64 SizeBytes);
	APRICOT_API void MemZero(void* Destination, uint64 SizeBytes);

	template<typename T, typename... Args>
	T* MemConstruct(void* Destination, Args&&... args)
	{
		return (T*)new (Destination) T(Forward<Args>(args)...);
	}

	/**
	* 
	*/
	class AMemoryArena : public AMalloc
	{
	public:
		enum class EFailureMode : uint8
		{
			None = 0,
			Assert, Fatal, Ignore,
			Max
		};

	public:
		virtual ~AMemoryArena() = default;

		virtual uint64 GetTotalSize() const = 0;
		virtual uint64 GetAllocatedSize() const = 0;
		virtual uint64 GetFreeSize() const = 0;
		virtual const TChar* GetDebugName() const = 0;

		FORCEINLINE EFailureMode GetFailureMode() const { return m_FailureMode; }
		FORCEINLINE void SetFailureMode(EFailureMode FailureMode) { m_FailureMode = FailureMode; }

	protected:
		EFailureMode m_FailureMode = EFailureMode::Ignore;
	};

}