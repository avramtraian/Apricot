// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#include "Apricot/Containers/Null.h"
#include "Apricot/Core/AObject.h"

#include "Apricot/Core/Memory/HeapAllocator.h"

namespace Apricot {

	/*
	* Apricot Engine
	* 
	* C++ Core engine architecture. Unique Pointer implementation.
	* 
	* @tparam T The complete type of the object.
	*/
	template<typename T>
	class TUniquePtr
	{
	/* Constructors & Deconstructor */
	public:
		TUniquePtr()
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TUniquePtr must derive from AObject!");
		}

		TUniquePtr(const TUniquePtr<T>&) = delete;

		TUniquePtr(TUniquePtr<T>&& Other) noexcept
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TUniquePtr must derive from AObject!");

			Other.m_Pointer = nullptr;
		}

		TUniquePtr(NullPlaceholder Null)
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TUniquePtr must derive from AObject!");
		}

		~TUniquePtr()
		{
			Release();
		}

	/* Overloaded operators */
	public:
		TUniquePtr<T>& operator=(const TUniquePtr<T>&) = delete;

		TUniquePtr<T>& operator=(TUniquePtr<T>&& Other) noexcept
		{
			Release();
			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
			return *this;
		}

		TUniquePtr<T>& operator=(NullPlaceholder Null)
		{
			Release();
			return *this;
		}

		T* operator->()
		{
			AE_DEBUG_CHECK(m_Pointer != nullptr); /* Pointer is null */
			return m_Pointer;
		}

		const T* operator->() const
		{
			AE_DEBUG_CHECK(m_Pointer != nullptr); /* Pointer is null */
			return m_Pointer;
		}

		T& operator*()
		{
			AE_DEBUG_CHECK(m_Pointer != nullptr); /* Pointer is null */
			return *m_Pointer;
		}

		const T& operator*() const
		{
			AE_DEBUG_CHECK(m_Pointer != nullptr); /* Pointer is null */
			return *m_Pointer;
		}

		operator bool8() const
		{
			return m_Pointer != nullptr;
		}

		bool8 operator==(const TUniquePtr<T>& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool8 operator!=(const TUniquePtr<T>& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}
	
	/* API functions */
	public:
		/*
		* Return the raw pointer.
		*/
		T* Get()
		{
			return m_Pointer;
		}

		const T* Get() const
		{
			return m_Pointer;
		}

		/*
		* Checks if the pointer is valid (not nullptr).
		*/
		bool8 IsValid() const
		{
			return m_Pointer != nullptr;
		}

		/*
		* Releases the object.
		* Currently, it will be immediately freed.
		*/
		void Release()
		{
			if (m_Pointer)
			{
				((AObject*)m_Pointer)->GetAllocator()->Free((void*)m_Pointer, sizeof(T), EAllocTag::UniquePtr);
				m_Pointer = nullptr;
			}
		}

	/* Casting */
	public:
		/*
		* Cast the pointer to another type.
		* 
		* @warning There is no type-safety, so there's nothing protecting the cast.
		*
		* @tparam R The type desired.
		*
		* @return An unknown state pointer. Might be valid or not.
		*/
		template<typename R>
		NODISCARD TUniquePtr<R> As() const
		{
			TUniquePtr<R> UniquePtr;
			// NOTE: Type-unsafe... Use a static_cast instead?
			UniquePtr.m_Pointer = (R*)m_Pointer;
			m_Pointer = nullptr;
			return UniquePtr;
		}

		/*
		* Cast the pointer to another type.
		*
		* @tparam R The type desired.
		*
		* @return nullptr in case the cast is not valid.
		*/
		template<typename R>
		NODISCARD TUniquePtr<R> DynamicAs() const
		{
			TUniquePtr<R> UniquePtr;
			UniquePtr.m_Pointer = dynamic_cast<R*>((TRemoveConst_Type<T>*)m_Pointer);
			if (UniquePtr.m_Pointer)
			{
				m_Pointer = nullptr;
			}
			return UniquePtr;
		}

	/* Raw Memory Access */
	/* NOTE: All raw memory controls are dangerous and must used only when they are absolutely needed. */
	public:
		/*
		* Set the pointer.
		*/
		FORCEINLINE void RawSetPointer(T* Pointer)
		{
			m_Pointer = Pointer;
		}

	private:
		/*
		* Pointer to the hold object.
		*/
		T* m_Pointer;
	};

	/*
	* Create a TUniquePtr, holding the newly created object.
	* It allocates the memory directly on the global heap. Use MakeUniqueWithAllocator for better control.
	* 
	* @tparam T The complete type of the object.
	* 
	* @param args The arguments used in construction of the object. They will be forwarded to the object constructor.
	* 
	* @return The TUniquePtr. It might return an invalid pointer if the memory allocation fails.
	*/
	template<typename T, typename... Args>
	constexpr TUniquePtr<T> MakeUnique(Args&&... args)
	{
		void* Memory = GHeapAllocator->Allocate(sizeof(T), EAllocTag::UniquePtr);

#ifdef AE_DEBUG
		if (!Memory)
		{
			// We should never get here.
			AE_DEBUG_CHECK(false);
			return TUniquePtr<T>();
		}
#endif

		T* Pointer = Memory_Placement<T>(Memory, Forward<Args>(args)...);
		((AObject*)Pointer)->SetAllocator(GHeapAllocator);
		return TUniquePtr<T>(Pointer);
	}

	/*
	* Create a TUniquePtr, holding the newly created object.
	* It allocates the memory via a custom allocator.
	*
	* @tparam T The complete type of the object.
	*
	* @param Allocator The allocator used for allocating the object's memory (and freeing it). If the request fails, this function will return an invalid pointer.
	* @param args The arguments used in construction of the object. They will be forwarded to the object constructor.
	*
	* @return The TUniquePtr. It might return an invalid pointer if the memory allocation fails.
	*/
	template<typename T, typename AllocatorType, typename... Args>
	constexpr TUniquePtr<T> MakeUniqueWithAllocator(AllocatorType* Allocator, Args&&... args)
	{
		void* Memory = Allocator->Allocate(sizeof(T), EAllocTag::UniquePtr);

#ifdef AE_DEBUG
		if (!Memory)
		{
			// Something went wrong with the allocator...
			// Invalid or corrupt?
			AE_DEBUG_CHECK(false);
			return TUniquePtr<T>();
		}
#endif

		T* Pointer = Memory_Placement<T>(Memory, Forward<Args>(args)...);
		((AObject*)Pointer)->SetAllocator(Allocator);
		return TUniquePtr<T>(Pointer);
	}

}