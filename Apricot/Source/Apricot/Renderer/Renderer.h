// Part of Apricot Engine. 2022-2022.
// Module: Renderer

#pragma once

#include "Apricot/Core/Base.h"
#include "Apricot/Core/Assert.h"

namespace Apricot {
	
	enum class ERenderingAPI : uint8
	{
		None   = 0,
		Vulkan = 1,
		D3D12  = 2,
		Metal  = 3,
		D3D11  = 4,
		OpenGL = 5
	};

	inline bool8 IsRenderingAPISupported(ERenderingAPI Api)
	{
		switch (Api)
		{
			case ERenderingAPI::None:
				return true;
			case ERenderingAPI::Vulkan:
#ifdef AE_VULKAN
				return true;
#else
				return false;
#endif
			case ERenderingAPI::D3D12:
#ifdef AE_D3D12
				return true;
#else
				return false;
#endif
			case ERenderingAPI::Metal:
#ifdef AE_METAL
				return true;
#else
				return false;
#endif
			case ERenderingAPI::D3D11:
#ifdef AE_D3D11
				return true;
#else
				return false;
#endif
			case ERenderingAPI::OpenGL:
#ifdef AE_OPENGL
				return true;
#else
				return false;
#endif
		}

		AE_CORE_ASSERT_NO_ENTRY(); // Invalid RenderingAPI!
		return false;
	}

}