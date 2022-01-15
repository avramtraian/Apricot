// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Apricot/Core/Base.h"

namespace Apricot {
	
	enum class RenderingAPI : uint8
	{
		None   = 0,
		Vulkan = 1,
		D3D12  = 2,
		Metal  = 3,
		D3D11  = 4,
		OpenGL = 5
	};

	inline bool IsRenderingAPISupported(RenderingAPI api)
	{
		switch (api)
		{
			case RenderingAPI::None:
				return true;
			case RenderingAPI::Vulkan:
#ifdef AE_VULKAN
				return true;
#else
				return false;
#endif
			case RenderingAPI::D3D12:
#ifdef AE_D3D12
				return true;
#else
				return false;
#endif
			case RenderingAPI::Metal:
#ifdef AE_METAL
				return true;
#else
				return false;
#endif
			case RenderingAPI::D3D11:
#ifdef AE_D3D11
				return true;
#else
				return false;
#endif
			case RenderingAPI::OpenGL:
#ifdef AE_OPENGL
				return true;
#else
				return false;
#endif
		}

		AE_CORE_ASSERT_RETURN(false, false, "Invalid RenderingAPI!");
		return false;
	}

}