// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#include "Apricot/Containers/Null.h"
#include "Apricot/Core/AObject.h"

namespace Apricot {

	/* Forward declarations */
	template<typename T>
	class TSharedPtr;
	template<typename T>
	class TWeakPtr;
	
	/*
	* Apricot Engine
	*
	* C++ Core engine architecture. Shared Reference implementation.
	* 
	* Similar to TSharedPtr, but it cannot hold a null value.
	* 
	* @tparam T The complete type of the object.
	*/
	template<typename T>
	class TSharedRef
	{
	/* Constructors & Deconstructor */
	public:
		TSharedRef(const TSharedRef<T>& Other)
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedRef must derive from AObject!");

			((TRemoveConst_Type<T>*)m_Pointer)->IncreaseReferenceCount();
		}

		TSharedRef(TSharedRef<T>&& Other) noexcept
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedRef must derive from AObject!");

			Other.m_Pointer = nullptr;
		}

		TSharedRef(NullPlaceholder Null)
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedRef must derive from AObject!");
		}

		~TSharedRef()
		{
			Release();
		}

	private:
		TSharedRef()
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TSharedRef must derive from AObject!");
		}

	/* Overloaded operators */
	public:
		TSharedRef<T>& operator=(const TSharedRef<T>& Other)
		{
			Release();

			m_Pointer = Other.m_Pointer;
			((TRemoveConst_Type<T>*)m_Pointer)->IncreaseReferenceCount();
			return *this;
		}

		TSharedRef<T>& operator=(TSharedRef<T>&& Other) noexcept
		{
			Release();

			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
			return *this;
		}

		TSharedRef<T>& operator=(NullPlaceholder Null)
		{
			Release();
			return *this;
		}

		T* operator->()
		{
			return m_Pointer;
		}

		const T* operator->() const
		{
			return m_Pointer;
		}

		T& operator*()
		{
			return *m_Pointer;
		}

		const T& operator*() const
		{
			return *m_Pointer;
		}

		bool8 operator==(const TSharedRef<T>& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool8 operator!=(const TSharedRef<T>& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}

	/* Conversion methods */
	public:
		/* Definitions found in 'PtrConversions.h' */
		/* You must include that file in order to use these. */
		TSharedPtr<T> ToSharedPtr() const;
		TWeakPtr<T> ToWeakPtr() const;

	/* API functions */
	public:
		T* Get()
		{
			return m_Pointer;
		}

		const T* Get() const
		{
			return m_Pointer;
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
		NODISCARD TSharedRef<R> As() const
		{
			TSharedRef<R> SharedRef;
			// NOTE: Type-unsafe... Use a static_cast instead?
			SharedRef.m_Pointer = (R*)m_Pointer;
			((TRemoveConst_Type<R>*)SharedRef.m_Pointer)->IncreaseReferenceCount();
			return SharedRef;
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

	/* Internal helper functions */
	private:
		/*
		* Releases the reference. Might cause the destruction of the object.
		*/
		void Release()
		{
			/* In case the TSharedRef was moved, we first check */
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

	private:
		/*
		* The pointer to the object.
		*/
		T* m_Pointer;

		friend class TSharedRef;
		friend class TSharedPtr<T>;
		friend class TWeakPtr<T>;
	};

}