// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "Apricot/Core/BaseTypes.h"

#include <new>

#define AE_MEMZERO_STRUCT(Struct) { ::Apricot::MemZero( &Struct, sizeof(  Struct )); }
#define AE_MEMZERO_ARRAY(Array)   { ::Apricot::MemZero(  Array,  sizeof(  Array  )); }
#define AE_MEMZERO_PTR(Ptr)       { ::Apricot::MemZero(  Ptr,    sizeof( *Ptr    )); }

#define AE_INVALID_MEMSIZE (static_cast<uint64>(-1))

/**
* 
*/
#define GetAlignmentOffset(MemoryAddress, Alignment) (((uint64)(Alignment) - (uintptr)(MemoryAddress) % (uint64)(Alignment)) % (uint64)(Alignment))

/**
*
*/
#define IsAddressBetween(Address, Begin, End) ( (uintptr)(Begin) <= (uintptr)(Address) && (uintptr)(Address) <= (uintptr)(End) )

#define AE_KILOBYTES(Kilobytes) ((Kilobytes) * 1024)
#define AE_MEGABYTES(Megabytes) ((Megabytes) * 1024 * 1024)
#define AE_GIGABYTES(Gigabytes) ((Gigabytes) * 1024 * 1024 * 1024)

namespace Apricot {

	/**
	* 
	*/
	enum class EMemoryError : int16
	{
		None = -1, Unknown = 0, Success = 1,

		OutOfMemory = -2,
		OutOfMemoryUnableToGrow = OutOfMemory             - 1,
		AlignmentIsForbidden    = OutOfMemoryUnableToGrow - 1,
		PointerOutOfRange       = AlignmentIsForbidden    - 1,
		AlreadyFreed            = PointerOutOfRange       - 1,
		InvalidArena            = AlreadyFreed            - 1,
		InvalidSize             = InvalidArena            - 1,
		InvalidAlignment        = InvalidSize             - 1,
		InvalidMemoryPtr        = InvalidAlignment        - 1,
		InvalidOuterPointer     = InvalidMemoryPtr        - 1,
		InvalidCall             = InvalidOuterPointer     - 1,
	};

	/**
	* 
	*/
	APRICOT_API void ApricotMemoryInit();
	APRICOT_API void ApricotMemoryDestroy();

	/**
	* 
	*/
	class APRICOT_API AMalloc
	{
	public:
		AMalloc();
		virtual ~AMalloc();

		NODISCARD virtual void* Alloc(uint64 Size, uint64 Alignment = sizeof(void*));
		NODISCARD virtual int32 TryAlloc(uint64 Size, void** OutPointer, uint64 Alignment = sizeof(void*));
		NODISCARD virtual void* AllocUnsafe(uint64 Size, uint64 Alignment = sizeof(void*));

		virtual void Free(void* Allocation, uint64 Size);
		virtual int32 TryFree(void* Allocation, uint64 Size);
		virtual void FreeUnsafe(void* Allocation, uint64 Size);
	};

	APRICOT_API extern AMalloc* GMalloc;

	APRICOT_API void MemCpy(void* Destination, const void* Source, uint64 SizeBytes);
	APRICOT_API void MemSet(void* Destination, int32 Value, uint64 SizeBytes);
	APRICOT_API void MemZero(void* Destination, uint64 SizeBytes);

	template<typename T, typename... Args>
	FORCEINLINE constexpr T* MemConstruct(void* Destination, Args&&... args)
	{
		return (T*)new (Destination) T(Forward<Args>(args)...);
	}

	/**
	* 
	*/
	class APRICOT_API AMemoryArena : public AMalloc
	{
	public:
		enum class EFailureMode : uint8
		{
			None = 0,
			Assert, Error, Ignore,
			Max
		};

	public:
		virtual ~AMemoryArena() = default;

		virtual void GarbageCollect() = 0;

		virtual const TChar* GetDebugName() const = 0;

		FORCEINLINE EFailureMode GetFailureMode() const { return m_FailureMode; }
		FORCEINLINE void SetFailureMode(EFailureMode FailureMode) { m_FailureMode = FailureMode; }

	protected:
		EFailureMode m_FailureMode = EFailureMode::Ignore;
	};

	enum class EAllocStrategy : uint8
	{
		BestFit, FirstFit
	};

}