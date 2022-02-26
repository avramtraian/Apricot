// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#include "Apricot/Containers/Null.h"
#include "Apricot/Core/AObject.h"

#include "Apricot/Core/Memory/HeapAllocator.h"

namespace Apricot {

	/* Forward declarations */
	template<typename T>
	class TSharedRef;
	template<typename T>
	class TWeakPtr;

	/*
	* Apricot Engine
	*
	* C++ Core engine architecture. Shared Pointer implementation.
	* 
	* @tparam T The complete type of the object.
	*/
	template<typename T>
	class TSharedPtr
	{
	/* Constructors & Deconstructor */
	public:
		TSharedPtr()
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedPtr must derive from AObject!");
		}

		TSharedPtr(const TSharedPtr<T>& Other)
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedPtr must derive from AObject!");

			if (m_Pointer)
			{
				((TRemoveConst_Type<T>*)m_Pointer)->IncreaseReferenceCount();
			}
		}

		TSharedPtr(TSharedPtr<T>&& Other) noexcept
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedPtr must derive from AObject!");

			Other.m_Pointer = nullptr;
		}

		TSharedPtr(NullPlaceholder Null)
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedPtr must derive from AObject!");
		}

		~TSharedPtr()
		{
			Release();
		}

	/* Overloaded operators */
	public:
		TSharedPtr<T>& operator=(const TSharedPtr<T>& Other)
		{
			Release();

			m_Pointer = Other.m_Pointer;
			if (m_Pointer)
			{
				((TRemoveConst_Type<T>*)m_Pointer)->IncreaseReferenceCount();
			}

			return *this;
		}

		TSharedPtr<T>& operator=(TSharedPtr<T>&& Other) noexcept
		{
			Release();

			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
			return *this;
		}

		TSharedPtr<T>& operator=(NullPlaceholder Null)
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

		bool8 operator==(const TSharedPtr<T>& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool8 operator!=(const TSharedPtr<T>& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}
	
	/* Conversion methods */
	public:
		/* Definitions found in 'PtrConversions.h' */
		/* You must include that file in order to use these. */
		TSharedRef<T> ToSharedRef() const;
		TWeakPtr<T> ToWeakPtr() const;

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
		* Check if the pointer is in a valid state.
		*/
		bool8 IsValid() const
		{
			return m_Pointer != nullptr;
		}

		/*
		* Releases the reference to the currently hold object, decreasing the reference count.
		* If the pointer is invalid (nullptr) nothing will happen.
		* If the reference count hits 0, the object will be destroyed.
		*/
		void Release()
		{
			if (m_Pointer)
			{
				((TRemoveConst_Type<T>*)m_Pointer)->DecreaseReferenceCount();
				if (m_Pointer->GetReferenceCount() == 0)
				{
					m_Pointer->~T();
					m_Pointer->GetAllocator()->Free((void*)m_Pointer, sizeof(T), EAllocTag::SharedPtr);
					m_Pointer = nullptr;
				}
			}
		}
	
	/* Casting */
	public:
		/*
		* Cast the pointer to another type.
		* 
		* @tparam R The type desired.
		* 
		* @warning There is currently no type-checking. So use this only when you know with what types are you working.
		* 
		* @return Always a casted pointer.
		*/
		template<typename R>
		NODISCARD TSharedPtr<R> As() const
		{
			TSharedPtr<R> SharedPtr;
			// NOTE: Type-unsafe... Use a static_cast instead?
			SharedPtr.m_Pointer = (R*)m_Pointer;
			if (SharedPtr.m_Pointer)
			{
				((TRemoveConst_Type<R>*)SharedPtr.m_Pointer)->IncreaseReferenceCount();
			}
			return SharedPtr;
		}

		/*
		* Cast the pointer to another type.
		* 
		* @tparam R The type desired.
		* 
		* @return nullptr in case the cast is not valid.
		*/
		template<typename R>
		NODISCARD TSharedPtr<R> DynamicAs() const
		{
			TSharedPtr<R> SharedPtr;
			SharedPtr.m_Pointer = dynamic_cast<R*>((TRemoveConst_Type<T>*)m_Pointer);
			if (SharedPtr.m_Pointer)
			{
				((TRemoveConst_Type<R>*)SharedPtr.m_Pointer)->IncreaseReferenceCount();
			}
			return SharedPtr;
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
		* The pointer to the object.
		*/
		T* m_Pointer;

		friend class TSharedPtr;
		friend class TSharedRef<T>;
		friend class TWeakPtr<T>;
	};

	/*
	* Create a TSharedPtr, holding the newly created object.
	* It allocates the memory directly on the global heap. Use MakeSharedWithAllocator for better control.
	*
	* @tparam T The complete type of the object.
	*
	* @param args The arguments used in construction of the object. They will be forwarded to the object constructor.
	*
	* @return The TSharedPtr. It might return an invalid pointer if the memory allocation fails.
	*/
	template<typename T, typename... Args>
	constexpr TSharedPtr<T> MakeShared(Args&&... args)
	{
		void* Memory = GHeapAllocator->Allocate(sizeof(T), EAllocTag::SharedPtr);

#ifdef AE_DEBUG
		if (!Memory)
		{
			// We should never get here.
			AE_DEBUG_CHECK(false);
			return TSharedPtr<T>();
		}
#endif

		TSharedPtr<T> SharedPtr;
		SharedPtr.RawSetPointer(MemConstruct<T>(Memory, Forward<Args>(args)...));
		((TRemoveConst_Type<T>*)SharedPtr.Get())->SetAllocator(GHeapAllocator);
		((TRemoveConst_Type<T>*)SharedPtr.Get())->IncreaseReferenceCount();
		return SharedPtr;
	}

	/*
	* Create a TSharedPtr, holding the newly created object.
	* It allocates the memory via a custom allocator.
	*
	* @tparam T The complete type of the object.
	*
	* @param Allocator The allocator used for allocating the object's memory (and freeing it). If the request fails, this function will return an invalid pointer.
	* @param args The arguments used in construction of the object. They will be forwarded to the object constructor.
	*
	* @return The TSharedPtr. It might return an invalid pointer if the memory allocation fails.
	*/
	template<typename T, typename AllocatorType, typename... Args>
	constexpr TSharedPtr<T> MakeSharedWithAllocator(AllocatorType* Allocator, Args&&... args)
	{
		void* Memory = Allocator->Allocate(sizeof(T), EAllocTag::SharedPtr);

#ifdef AE_DEBUG
		if (!Memory)
		{
			// Something went wrong with the allocator...
			// Invalid or corrupt?
			AE_DEBUG_CHECK(false);
			return TSharedPtr<T>();
		}
#endif

		TSharedPtr<T> SharedPtr;
		SharedPtr.RawSetPointer(Memory_Placement<T>(Memory, Forward<Args>(args)...));
		((TRemoveConst_Type<T>*)SharedPtr.Get())->SetAllocator(Allocator);
		((TRemoveConst_Type<T>*)SharedPtr.Get())->IncreaseReferenceCount();
		return SharedPtr;
	}

}