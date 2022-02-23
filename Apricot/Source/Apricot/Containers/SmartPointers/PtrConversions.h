// Part of Apricot Engine. 2022-2022.

#pragma once

#include "SharedPtr.h"
#include "SharedRef.h"
#include "WeakPtr.h"

namespace Apricot {
	
	template<typename T>
	TSharedRef<T> TSharedPtr<T>::ToSharedRef() const
	{
		AE_DEBUG_CHECK(m_Pointer != nullptr); /* A TSharedRef must always be valid */

		TSharedRef<T> SharedRef;
		SharedRef.m_Pointer = m_Pointer;
		((TRemoveConst_Type<T>*)SharedRef.m_Pointer)->IncreaseReferenceCount();
		return SharedRef;
	}

	template<typename T>
	TWeakPtr<T> TSharedPtr<T>::ToWeakPtr() const
	{
		TWeakPtr<T> WeakPtr;
		WeakPtr.m_Pointer = m_Pointer;
		return WeakPtr;
	}

	template<typename T>
	TSharedPtr<T> TSharedRef<T>::ToSharedPtr() const
	{
		TSharedPtr<T> SharedPtr;
		SharedPtr.m_Pointer = m_Pointer;
		((TRemoveConst_Type<T>*)SharedPtr.m_Pointer)->IncreaseReferenceCount();
		return SharedPtr;
	}

	template<typename T>
	TWeakPtr<T> TSharedRef<T>::ToWeakPtr() const
	{
		TWeakPtr<T> WeakPtr;
		WeakPtr.m_Pointer = m_Pointer;
		return WeakPtr;
	}

	template<typename T>
	TSharedPtr<T> TWeakPtr<T>::ToSharedPtr() const
	{
		TSharedPtr<T> SharedPtr;
		SharedPtr.m_Pointer = m_Pointer;
		if (SharedPtr.m_Pointer)
		{
			((TRemoveConst_Type<T>*)SharedPtr.m_Pointer)->IncreaseReferenceCount();
		}
		return SharedPtr;
	}

	template<typename T>
	TSharedRef<T> TWeakPtr<T>::ToSharedRef() const
	{
		AE_DEBUG_CHECK(m_Pointer != nullptr); /* A TSharedRef must always be valid */

		TSharedRef<T> SharedRef;
		SharedRef.m_Pointer = m_Pointer;
		((TRemoveConst_Type<T>*)SharedRef.m_Pointer)->IncreaseReferenceCount();
		return SharedRef;
	}

}