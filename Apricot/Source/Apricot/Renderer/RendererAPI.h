#pragma once

#include "Apricot/Core/Base.h"

#if defined(AE_PLATFORM_WINDOWS)
	#define AE_VULKAN
	#define AE_D3D12
	#define AE_D3D11
	#define AE_OPENGL
#endif

namespace Apricot {

	enum class RenderingAPI
	{
		Invalid = 0,
		None, Vulkan, D3D12, Metal, D3D11, OpenGL,
		MaxEnumValue
	};

	class APRICOT_API RendererAPI
	{
	public:
		static RenderingAPI GetAPI() { return s_API; }
		static void SetAPI(RenderingAPI API) { s_API = API; }

		static bool IsAPISupported(RenderingAPI API)
		{
			switch (API)
			{
				case RenderingAPI::None:
				{
					return true;
				}
				case RenderingAPI::Vulkan:
				{
					#ifdef AE_VULKAN
						return true;
					#else
						return false;
					#endif
				}
				case RenderingAPI::D3D12:
				{
					#ifdef AE_D3D12
						return true;
					#else
						return false;
					#endif
				}
				case RenderingAPI::Metal:
				{
					#ifdef AE_METAL
						return true;
					#else
						return false;
					#endif
				}
				case RenderingAPI::D3D11:
				{
					#ifdef AE_D3D11
						return true;
					#else
						return false;
					#endif
				}
				case RenderingAPI::OpenGL:
				{
					#ifdef AE_OPENGL
						return true;
					#else
						return false;
					#endif
				}
			}

			return false;
		}

	private:
		static inline RenderingAPI s_API = RenderingAPI::None;
	};

}