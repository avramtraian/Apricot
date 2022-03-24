#pragma once

#ifdef AE_COMPILER_MSVC
	
	using uint8   = unsigned char;
	using uint16  = unsigned short;
	using uint32  = unsigned int;
	using uint64  = unsigned long long;
				  
	using int8    = signed char;
	using int16   = signed short;
	using int32   = signed int;
	using int64   = signed long long;
				  
	using char8   = char;
	using char16  = wchar_t;

	using float32 = float;
	using float64 = double;

	using bool8   = bool;
	using bool32  = int;

	using uintptr = uint64;

#else
	#error "Unknown or unsupported compiler!"

	using uint8   = unsigned char;
	using uint16  = unsigned short;
	using uint32  = unsigned int;
	using uint64  = unsigned long long;
				  
	using int8    = signed char;
	using int16   = signed short;
	using int32   = signed int;
	using int64   = signed long long;

	using char8   = char;
	using char16  = wchar_t;

	using float32 = float;
	using float64 = double;

	using bool8   = bool;
	using bool32  = int;

	using uintptr = uint64;

#endif