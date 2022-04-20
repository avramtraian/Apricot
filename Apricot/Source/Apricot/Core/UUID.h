#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API UUID
	{
	public:
		static inline uint64 Invalid = 0;

	public:
		UUID();
		UUID(uint64 Uuid);
		UUID& operator=(uint64 Uuid);

		UUID(const UUID&) = default;
		UUID(UUID&&) noexcept = default;

		UUID& operator=(const UUID&) = default;
		UUID& operator=(UUID&&) noexcept = default;

		~UUID() = default;

	public:
		operator uint64() const { return m_UUID; }

	private:
		uint64 m_UUID;
	};

}