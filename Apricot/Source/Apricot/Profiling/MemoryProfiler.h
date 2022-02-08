// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/BaseTypes.h"

#ifdef AE_ENABLE_MEMORY_TRACE

namespace Apricot {

	class APRICOT_API AMemoryProfiler
	{
	public:
		static void Init();
		static void Destroy();

		void SubmitLowLevelMalloc(uint64 Size, const TChar* File, const TChar* Func, uint64 Line);
		void SubmitLowLevelFree(uint64 Size, const TChar* File, const TChar* Func, uint64 Line);

	private:
		AMemoryProfiler() = default;
		AMemoryProfiler(const AMemoryProfiler&) = delete;
		AMemoryProfiler& operator=(const AMemoryProfiler&) = delete;
		~AMemoryProfiler() = default;

		template<typename T, typename... Args>
		friend T* MemConstruct(void*, Args&&...);
	};

	APRICOT_API extern AMemoryProfiler* GMemoryProfiler;

}

#endif