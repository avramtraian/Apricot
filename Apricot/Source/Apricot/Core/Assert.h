#pragma once

#include "Base.h"

#define AE_CORE_ASSERT(Condition, Message) if (!(Condition)) { AE_DEBUGBREAK(); }