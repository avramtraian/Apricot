#pragma once

#include "Base.h"
#include "Assert.h"
#include "Memory.h"

#include <type_traits>

namespace Apricot {

	template<typename T>
	class WeakRef;

	class APRICOT_API RefCounted
	{
	public:
		RefCounted() = default;

	private:
		uint64 m_ReferenceCount = 0;

		template<typename T>
		friend class Ref;
	};

	template<typename T>
	class Ref
	{
	public:
		static_assert(std::is_base_of<RefCounted, T>::value, "Ref<T> - T is not derived from RefCounted!");

		Ref()
			: m_Pointer(nullptr)
		{
		}

		Ref(const Ref& other)
			: m_Pointer(other.m_Pointer)
		{
			if (m_Pointer)
			{
				IncrementRef();
			}
		}

		Ref(Ref&& other) noexcept
			: m_Pointer(other.m_Pointer)
		{
			other.m_Pointer = nullptr;
		}

		explicit Ref(T* pointer)
			: m_Pointer(pointer)
		{
			if (m_Pointer)
			{
				IncrementRef();
			}
		}

		Ref(NullptrType)
			: m_Pointer(nullptr)
		{
		}

		~Ref()
		{
			if (m_Pointer)
			{
				DecrementRef();
			}
		}

	public:
		Ref& operator=(const Ref& other)
		{
			if (other.m_Pointer)
			{
				IncrementRef();
			}
			if (m_Pointer)
			{
				DecrementRef();
			}
			m_Pointer = other.m_Pointer;
			return *this;
		}

		Ref& operator=(Ref&& other) noexcept
		{
			AE_CORE_ASSERT(this != &other); // Moving a Ref<T> to himself could result in undefined behavior

			if (m_Pointer)
			{
				DecrementRef();
			}
			m_Pointer = other.m_Pointer;
			other.m_Pointer = nullptr;
			return *this;
		}

		Ref& operator=(NullptrType)
		{
			Release();
			return *this;
		}

		T* operator->()
		{
			AE_CORE_ASSERT(m_Pointer != nullptr); // Trying to dereference an invalid pointer
			return m_Pointer;
		}

		const T* operator->() const
		{
			AE_CORE_ASSERT(m_Pointer != nullptr); // Trying to dereference an invalid pointer
			return m_Pointer;
		}

		T& operator*()
		{
			AE_CORE_ASSERT(m_Pointer != nullptr); // Trying to dereference an invalid pointer
			return *m_Pointer;
		}

		const T& operator*() const
		{
			AE_CORE_ASSERT(m_Pointer != nullptr); // Trying to dereference an invalid pointer
			return *m_Pointer;
		}

		operator bool() const
		{
			return IsValid();
		}

		bool operator==(const Ref& other) const
		{
			return m_Pointer == other.m_Pointer;
		}

		bool operator!=(const Ref& other) const
		{
			return !(*this == other);
		}

	public:
		template<typename... Args>
		static Ref Create(Args&&... args)
		{
			return Ref<T>(anew T(astl::forward<Args>(args)...));
		}

	public:
		T* Get() const
		{
			return m_Pointer;
		}

		bool IsValid() const
		{
			return m_Pointer != nullptr;
		}

		void Release()
		{
			if (m_Pointer)
			{
				DecrementRef();
				m_Pointer = nullptr;
			}
		}

	private:
		void IncrementRef()
		{
			m_Pointer->m_ReferenceCount++;
		}

		void DecrementRef()
		{
			if (m_Pointer->m_ReferenceCount <= 1)
			{
				adelete m_Pointer;
			}
			else
			{
				m_Pointer->m_ReferenceCount--;
			}
		}

	private:
		T* m_Pointer;

		template<typename FriendT>
		friend class WeakRef;
	};

}