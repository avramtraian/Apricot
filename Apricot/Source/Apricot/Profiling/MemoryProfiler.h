// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/BaseTypes.h"

#ifdef AE_ENABLE_MEMORY_TRACE

namespace Apricot {

	class APRICOT_API AMemoryProfiler
	{
	private:
		AMemoryProfiler();
		AMemoryProfiler(const AMemoryProfiler&) = delete;
		AMemoryProfiler& operator=(const AMemoryProfiler&) = delete;
		~AMemoryProfiler() = default;

	public:
		static void Init();
		static void Destroy();

		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};

	APRICOT_API extern AMemoryProfiler* GMemoryProfiler;

}

#endif