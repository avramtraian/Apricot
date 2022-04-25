#pragma once

#include "Base.h"

namespace Apricot {

	class APRICOT_API CrashReporter
	{
	public:
		static void Init();
		static void Destroy();

		static void OnCoreAssert(const char* Expression, const char* File, uint32 Line);
	};

}