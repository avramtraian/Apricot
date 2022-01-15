// Part of Apricot Engine. 2022-2022.

#include "aepch.h"
#include "EngineConfiguration.h"

#include "Engine.h"

#include "Apricot/Renderer/Renderer.h"

namespace Apricot {

	namespace Utils {
		
		template<uint64 S>
		static void ParseFlag(const TStackString<char8, S>& fullFlag)
		{
			if (fullFlag[0] != '-')
			{
				AE_BASE_ASSERT_RETURN(false, AE_VOID, "All command-line flags must begin with '-'!");
				return;
			}

			uint64 colon = fullFlag.Find(":");
			TStackString<char8, S> flag;
			TStackString<char8, S> value;

			if (colon == AE_STRING_NPOS)
			{
				flag = fullFlag.ToLowerCase().Substr(1);
			}
			else
			{
				flag = fullFlag.Substr(1, colon).ToLowerCase();
				value = fullFlag.Substr(colon + 1).ToLowerCase();
			}

			if (flag == "noconsole")
			{
				GEngine->Config.bStartWithConsole = false;
			}
			else if (flag == "vulkan")
			{
				AE_BASE_ASSERT_RETURN(IsRenderingAPISupported(RenderingAPI::Vulkan), AE_VOID, "RenderingAPI Vulkan is not supported on the current platform.");
				GEngine->Config.RenderingAPI = (uint8)RenderingAPI::Vulkan;
			}
			else if (flag == "d3d12")
			{
				AE_BASE_ASSERT_RETURN(IsRenderingAPISupported(RenderingAPI::D3D12), AE_VOID, "RenderingAPI D3D12 is not supported on the current platform.");
				GEngine->Config.RenderingAPI = (uint8)RenderingAPI::D3D12;
			}
			else if (flag == "metal")
			{
				AE_BASE_ASSERT_RETURN(IsRenderingAPISupported(RenderingAPI::Metal), AE_VOID, "RenderingAPI Metal is not supported on the current platform.");
				GEngine->Config.RenderingAPI = (uint8)RenderingAPI::Metal;
			}
			else if (flag == "d3d11")
			{
				AE_BASE_ASSERT_RETURN(IsRenderingAPISupported(RenderingAPI::D3D11), AE_VOID, "RenderingAPI D3D11 is not supported on the current platform.");
				GEngine->Config.RenderingAPI = (uint8)RenderingAPI::D3D11;
			}
			else if (flag == "opengl")
			{
				AE_BASE_ASSERT_RETURN(IsRenderingAPISupported(RenderingAPI::OpenGL), AE_VOID, "RenderingAPI OpenGL is not supported on the current platform.");
				GEngine->Config.RenderingAPI = (uint8)RenderingAPI::OpenGL;
			}
			else
			{
				AE_BASE_ASSERT_RETURN(false, AE_VOID, "Unknown command-line flag!");
			}
		}

	}

	void EngineConfig::Init(const char8* commandLine)
	{
		// Parse command-line flags.
		String256 cmdLine = commandLine;
		cmdLine.ForEachWord<>([](String256& word)
			{
				Utils::ParseFlag(word);
			});

		// TODO: Read configuration files.
		// ...
	}

	void EngineConfig::Destroy()
	{
		
	}

#ifdef AE_PLATFORM_WINDOWS
	
	bool8 ConfigSerializer::ReadGlobalEngineConfig(const Filesystem::Path256& filepath)
	{
		return false;
	}

	bool8 ConfigSerializer::WriteGlobalEngineConfig(const Filesystem::Path256& filepath)
	{
		return false;
	}

#endif

}