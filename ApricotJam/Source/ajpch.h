// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Config.h"

#ifdef AE_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <type_traits>

#include <Apricot/Core/Base.h>
#include <Apricot/Core/Types.h>
#include <Apricot/Core/Assert.h>

#include <Apricot/Containers/SmartPointers/SharedPtr.h>
#include <Apricot/Containers/SmartPointers/SharedRef.h>
#include <Apricot/Containers/SmartPointers/WeakPtr.h>
#include <Apricot/Containers/SmartPointers/UniquePtr.h>
#include <Apricot/Containers/SmartPointers/PtrConversions.h>

#include <Apricot/Containers/Array.h>
#include <Apricot/Containers/Vector.h>
#include <Apricot/Containers/String.h>
#include <Apricot/Containers/StringClass.h>