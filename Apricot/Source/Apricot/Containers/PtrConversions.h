// Part of Apricot Engine. 2022-2022.
// Submodule: Containers

#pragma once

#include "SharedPtr.h"
#include "SharedRef.h"
#include "WeakPtr.h"

namespace Apricot {
	
	template<typename T>
	TSharedRef<T> TSharedPtr<T>::ToSharedRef() const
	{
		AE_CORE_ASSERT(m_Pointer != nullptr); /* A TSharedRef must always be valid */

		TSharedRef<T> SharedRef;
		SharedRef.m_Pointer = m_Pointer;
		SharedRef.m_Pointer->__m_ReferenceCount++;
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
		SharedPtr.m_Pointer->__m_ReferenceCount++;
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
			SharedPtr.m_Pointer->__m_ReferenceCount++;
		}
		return SharedPtr;
	}

	template<typename T>
	TSharedRef<T> TWeakPtr<T>::ToSharedRef() const
	{
		AE_CORE_ASSERT(m_Pointer != nullptr); /* A TSharedRef must always be valid */

		TSharedRef<T> SharedRef;
		SharedRef.m_Pointer = m_Pointer;
		SharedRef.m_Pointer->__m_ReferenceCount++;
		return SharedRef;
	}

}