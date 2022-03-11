// Part of Apricot Engine. 2022-2022.
// Submodule: Profiling

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot {

	/**
	* C++ Core Profiling Tool
	* 
	* Keeps track of memory allocation, providing EDITOR & RUNTIME memory information
	*/
	class APRICOT_API AMemoryProfiler
	{
	/* Constructors & Deconstructor */
	private:
		AMemoryProfiler();
		AMemoryProfiler(const AMemoryProfiler&) = delete;
		AMemoryProfiler& operator=(const AMemoryProfiler&) = delete;
		~AMemoryProfiler() = default;

	/* Init & Destroy */
	public:
		static void Init();
		static void Destroy();

		static void SubmitHeapAllocation(uint64 size, uint64 alignment);
		static void SubmitHeapDeallocation(void* block, uint64 size);

	/* Friends */
	private:
		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};

	APRICOT_API extern AMemoryProfiler* GMemoryProfiler;

}