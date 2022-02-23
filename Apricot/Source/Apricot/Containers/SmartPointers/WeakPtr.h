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
	class TSharedRef;

	/*
	* Apricot Engine
	*
	* C++ Core engine architecture. Weak Shared Pointer implementation.
	* 
	* Its main feature is that it doesn't increase or decrease the object's reference count.
	* 
	* @tparam T The complete type of the object.
	*/
	template<typename T>
	class TWeakPtr
	{
	/* Constructors & Deconstructor */
	public:
		TWeakPtr()
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TWeakPtr must derive from AObject!");
		}

		TWeakPtr(const TWeakPtr<T>& Other)
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TWeakPtr must derive from AObject!");
		}

		TWeakPtr(TWeakPtr<T>&& Other) noexcept
			: m_Pointer(Other.m_Pointer)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TWeakPtr must derive from AObject!");

			Other.m_Pointer = nullptr;
		}

		TWeakPtr(NullPlaceholder Null)
			: m_Pointer(nullptr)
		{
			AE_STATIC_ASSERT(IsDerivedFrom<T, AObject>(), "Classes used with TWeakPtr must derive from AObject!");
		}

		~TWeakPtr()
		{
		}

	/* Overloaded operators */
	public:
		TWeakPtr<T>& operator=(const TWeakPtr<T>& Other)
		{
			m_Pointer = Other.m_Pointer;
			return *this;
		}

		TWeakPtr<T>& operator=(TWeakPtr<T>&& Other) noexcept
		{
			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
			return *this;
		}

		TWeakPtr<T>& operator=(NullPlaceholder Null)
		{
			m_Pointer = nullptr;
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

		bool8 operator==(const TWeakPtr<T>& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool8 operator!=(const TWeakPtr<T>& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}

	/* Conversion methods */
	public:
		/* Definitions found in 'PtrConversions.h' */
		/* You must include that file in order to use these. */
		TSharedPtr<T> ToSharedPtr() const;
		TSharedRef<T> ToSharedRef() const;

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

		bool8 IsValid() const
		{
			return m_Pointer != nullptr;
		}

		void Release()
		{
			m_Pointer = nullptr;
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
		NODISCARD TWeakPtr<R> As() const
		{
			TWeakPtr<R> WeakPtr;
			// NOTE: Type-unsafe... Use a static_cast instead?
			WeakPtr.m_Pointer = (R*)m_Pointer;
			return WeakPtr;
		}

		/*
		* Cast the pointer to another type.
		*
		* @tparam R The type desired.
		*
		* @return nullptr in case the cast is not valid.
		*/
		template<typename R>
		NODISCARD TWeakPtr<R> DynamicAs() const
		{
			TWeakPtr<R> WeakPtr;
			WeakPtr.m_Pointer = dynamic_cast<R*>((TRemoveConst_Type<T>*)m_Pointer);
			return WeakPtr;
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

		friend class TWeakPtr;
		friend class TSharedPtr<T>;
		friend class TSharedRef<T>;
	};

}