#pragma once

#include "Apricot/Core/Base.h"

#include <ASTL/vector.h>
#include <ASTL/array.h>
#include <ASTL/cstring.h>
#include <ASTL/string.h>
#include <ASTL/string_view.h>
#include <ASTL/hash_table.h>
#include <ASTL/hash_set.h>
#include <ASTL/map.h>
#include <ASTL/unordered_map.h>
#include <ASTL/unordered_set.h>
#include <ASTL/memory.h>

#include "Apricot/Core/Log.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"
#include "Apricot/Core/Ref.h"

#ifdef AE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif