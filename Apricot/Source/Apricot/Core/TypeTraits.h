#pragma once

#include "Base.h"

namespace Apricot {

	template<typename T>
	struct RemoveReference
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveReference<T&>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveReference<T&&>
	{
		using Type = T;
	};

	template<typename T>
	using RemoveReferenceType = typename RemoveReference<T>::Type;

	template<typename T>
	constexpr RemoveReferenceType<T>&& Move(T&& Object) noexcept
	{
		return static_cast<RemoveReferenceType<T>&&>(Object);
	}

	template<typename T>
	constexpr T&& Forward(RemoveReferenceType<T>& Object) noexcept
	{
		return static_cast<T&&>(Object);
	}

}