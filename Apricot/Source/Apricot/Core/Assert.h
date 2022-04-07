#pragma once

#include "Base.h"

#define AE_CORE_ASSERT(Condition, ...) if (!(Condition)) { AE_DEBUGBREAK(); }