#pragma once

#include "Ref.h"

#include <type_traits>

namespace Apricot {

	template<typename T>
	class WeakRef
	{
	public:
		static_assert(std::is_base_of<RefCounted, T>::value, "WeakRef<T> - T is not derived from RefCounted!");

		WeakRef()
			: m_Pointer(nullptr)
		{
		}

		WeakRef(const WeakRef& Other)
			: m_Pointer(Other.m_Pointer)
		{
		}

		WeakRef(WeakRef&& Other) noexcept
			: m_Pointer(Other.m_Pointer)
		{
			Other.m_Pointer = nullptr;
		}

		WeakRef(const Ref<T>& Other)
			: m_Pointer(Other.m_Pointer)
		{
		}

		WeakRef(NullptrType)
			: m_Pointer(nullptr)
		{
		}

		~WeakRef()
		{
		}

	public:
		WeakRef& operator=(const WeakRef& Other)
		{
			m_Pointer = Other.m_Pointer;
			return *this;
		}

		WeakRef& operator=(WeakRef&& Other) noexcept
		{
			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
			return *this;
		}

		WeakRef& operator=(const Ref<T>& Other)
		{
			m_Pointer = Other.m_Pointer;
			return *this;
		}

		WeakRef& operator=(NullptrType)
		{
			m_Pointer = nullptr;
			return *this;
		}

		T* operator->()
		{
			AE_CORE_ASSERT(m_Pointer != nullptr);
			return m_Pointer;
		}

		const T* operator->() const
		{
			AE_CORE_ASSERT(m_Pointer != nullptr);
			return m_Pointer;
		}

		T& operator*()
		{
			AE_CORE_ASSERT(m_Pointer != nullptr);
			return *m_Pointer;
		}

		const T& operator*() const
		{
			AE_CORE_ASSERT(m_Pointer != nullptr);
			return *m_Pointer;
		}

		operator bool() const
		{
			return IsValid();
		}

		bool operator==(const WeakRef& Other) const
		{
			return m_Pointer == Other.m_Pointer;
		}

		bool operator!=(const WeakRef& Other) const
		{
			return m_Pointer != Other.m_Pointer;
		}

	public:
		T* Get() { return m_Pointer; }
		const T* Get() const { return m_Pointer; }

		void Release()
		{
			m_Pointer = nullptr;
		}

		bool IsValid() const
		{
			return m_Pointer != nullptr;
		}

	private:
		T* m_Pointer;

		template<typename FriendT>
		friend class Ref;
	};

}