#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/TypeTraits.h"
#include "Apricot/Core/Memory.h"

namespace Apricot {

	template<typename T>
	class Scope
	{
	public:
		template<typename... Args>
		static Scope<T> Create(Args&&... args)
		{
			return Scope<T>(anew T(Apricot::Forward<Args>(args)...));
		}

	public:
		Scope()
			: m_Pointer(nullptr)
		{
		}

		Scope(const Scope&) = delete;

		Scope(Scope&& Other) noexcept
		{
			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;
		}

		Scope(NullptrType)
			: m_Pointer(nullptr)
		{
		}

		~Scope()
		{
			Release();
		}

	private:
		Scope(T* Pointer)
			: m_Pointer(Pointer)
		{
		}

	public:
		Scope& operator=(const Scope&) = delete;

		Scope& operator=(Scope&& Other) noexcept
		{
			Release();

			m_Pointer = Other.m_Pointer;
			Other.m_Pointer = nullptr;

			return *this;
		}

		Scope& operator=(NullptrType)
		{
			Release();
			return *this;
		}

		T* operator->()
		{
			AE_CORE_ASSERT(IsValid());
			return m_Pointer;
		}

		const T* operator->() const
		{
			AE_CORE_ASSERT(IsValid());
			return m_Pointer;
		}

		T& operator*()
		{
			AE_CORE_ASSERT(IsValid());
			return *m_Pointer;
		}

		const T& operator*() const
		{
			AE_CORE_ASSERT(IsValid());
			return *m_Pointer;
		}

		operator bool() const
		{
			return IsValid();
		}

	public:
		T* Get() { return m_Pointer; }
		const T* Get() const { return m_Pointer; }

		void Release()
		{
			if (m_Pointer)
			{
				adelete m_Pointer;
			}
		}

		bool IsValid() const
		{
			return m_Pointer != nullptr;
		}

	private:
		T* m_Pointer;
	};

}