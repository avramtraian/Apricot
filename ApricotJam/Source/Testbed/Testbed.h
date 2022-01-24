// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot { namespace Testbed {

	bool8 Vector_Create_Destroy();
	bool8 Vector_Create_Destroy_Custom_Allocators();

	bool8 Vector_Push_Emplace_Pop_Back();
	bool8 Vector_Push_Emplace_Pop_Back_Custom_Allocators();

	bool8 Vector_Copy_Move();
	bool8 Vector_Copy_Move_Custom_Allocators();

	bool8 Vector_Clear_Tidy_Resize_Reserve();
	bool8 Vector_Clear_Tidy_Resize_Reserve_Custom_Allocators();

	void Run_Vector_Tests();

} }