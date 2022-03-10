// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"

#define ACLASS_CORE() \
	private: \
		uint64 __m_ReferenceCount = 0; \
	private: \
		template<typename T> \
		friend class TSharedPtr; \
		template<typename T> \
		friend class TSharedRef; \
		template<typename T> \
		friend class TWeakPtr; \
		template<typename T> \
		friend class TUniquePtr; \
		\
		template<typename T, typename... Args> \
		friend constexpr TSharedPtr<T> MakeShared(Args&&...); \
		\
		template<typename T> \
		friend TSharedRef<T> TSharedPtr<T>::ToSharedRef() const; \
		template<typename T> \
		friend TWeakPtr<T> TSharedPtr<T>::ToWeakPtr() const; \
		template<typename T> \
		friend TSharedPtr<T> TSharedRef<T>::ToSharedPtr() const; \
		template<typename T> \
		friend TWeakPtr<T> TSharedRef<T>::ToWeakPtr() const; \
		template<typename T> \
		friend TSharedPtr<T> TWeakPtr<T>::ToSharedPtr() const; \
		template<typename T> \
		friend TSharedRef<T> TWeakPtr<T>::ToSharedRef() const;