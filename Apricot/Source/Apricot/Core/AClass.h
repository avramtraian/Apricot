// Part of Apricot Engine. 2022-2022.
// Module: Core

#pragma once

#include "Base.h"
#include "Types.h"

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
		friend constexpr TSharedPtr<T> MakeShared(Args&&...);
