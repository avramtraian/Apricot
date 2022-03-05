// Part of Apricot Engine. 2022-2022.
// Submodule: Profiling

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Types.h"

#include "ScopedTimer.h"

#define AE_SCOPE_PERFORMANCE(ScopeName) ::Apricot::ScopedTimer AE_CONCATENATE(___SCOPETIMER, AE_LINE) = ::Apricot::ScopedTimer(TEXT(ScopeName))