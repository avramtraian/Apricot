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
		// Read configurations files.
		TArray<Platform::ConfigFileKey, 2> configs;
		char16 buffer[128] = { 0 };

		configs[0] =
		{
			/* Section */ L"Core",
			/* Key     */ L"bStartWithConsole",
			/* Default */ L"True",
			buffer, sizeof(buffer) / sizeof(char16)};

		configs[1] =
		{
			/* Section */ L"Renderer_Core",
			/* Key     */ L"RenderingAPI",
			/* Default */ L"None",
			buffer, sizeof(buffer) / sizeof(char16) };

		Platform::ReadConfigFile(L"../Config/Engine.ini", configs.Data(), configs.Size());

		WString128 value;

		value = configs[0].Value;
		value = value.ToLowerCase();
		if (value == L"true")
		{
			GEngine->Config.bStartWithConsole = true;
		}
		else if (value == L"false")
		{
			GEngine->Config.bStartWithConsole = false;
		}

		value = configs[1].Value;
		value = value.ToLowerCase();
		if (value == L"none")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::None;
		}
		else if (value == L"vulkan")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::Vulkan;
		}
		else if (value == L"d3d12")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::D3D12;
		}
		else if (value == L"metal")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::Metal;
		}
		else if (value == L"d3d11")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::D3D11;
		}
		else if (value == L"opengl")
		{
			GEngine->Config.RenderingAPI = (uint8)RenderingAPI::OpenGL;
		}

		// Parse command-line flags.
		String256 cmdLine = commandLine;
		cmdLine.ForEachWord<>([](String256& word)
			{
				Utils::ParseFlag(word);
			});

		Flush();
	}

	void EngineConfig::Destroy()
	{
		Flush();
	}

	void EngineConfig::Flush()
	{
		TArray<Platform::ConfigFileKey, 2> configs;
		uint16 currentIndex = 0;

		// bStartWithConsole
		if (GEngine->Config.bHasChanged_bStartWithConsole)
		{
			if (GEngine->Config.bStartWithConsole)
			{
				configs[currentIndex] = { L"Core", L"bStartWithConsole", nullptr, L"True", 0 };
			}
			else
			{
				configs[currentIndex] = { L"Core", L"bStartWithConsole", nullptr, L"False", 0 };
			}
			currentIndex++;
		}

		// RenderingAPI
		if (GEngine->Config.bHasChanged_RenderingAPI)
		{
			switch (GEngine->Config.RenderingAPI)
			{
				case (uint8)RenderingAPI::None:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"None", 0 };
					break;
				}
				case (uint8)RenderingAPI::Vulkan:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"Vulkan", 0 };
					break;
				}
				case (uint8)RenderingAPI::D3D12:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"D3D12", 0 };
					break;
				}
				case (uint8)RenderingAPI::Metal:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"Metal", 0 };
					break;
				}
				case (uint8)RenderingAPI::D3D11:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"D3D11", 0 };
					break;
				}
				case (uint8)RenderingAPI::OpenGL:
				{
					configs[currentIndex] = { L"Renderer_Core", L"RenderingAPI", nullptr, L"OpenGL", 0 };
					break;
				}
			}
			currentIndex++;
		}

		Platform::WriteConfigFile(L"../Config/Engine.ini", configs.Data(), currentIndex);
	}

}