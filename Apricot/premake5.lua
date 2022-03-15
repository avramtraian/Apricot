project "AE"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	pchheader "aepch.h"
	pchsource "Source/aepch.cpp"

	files {
		"Source/**.h",
		"Source/**.cpp",

		"%{IncludeDirs.Optick}/**.h",
		"%{IncludeDirs.VulkanSDK}/**.h",
		"%{IncludeDirs.VulkanSDK}/**.hpp"
	}

	includedirs {
		"Source",

		"%{IncludeDirs.Optick}",
		"%{IncludeDirs.VulkanSDK}"
	}

	defines {
		"AE_EXPORT_DLL"
	}

	libdirs {
		"%{LibraryDirs.VulkanSDK}"
	}

	links {
		"%{Libraries.Vulkan}"
	}

	filter { "system:windows" }
		systemversion "latest"
		characterset "Unicode"
		defines {
			"AE_PLATFORM_WINDOWS",
			"AE_UNICODE"
		}

	filter { "configurations:Debug_Editor" }
		defines {
			"AE_CONFIG_DEBUG_EDITOR"
		}

		links {
			"%{Libraries.Optick}"
		}

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-DebugEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"%{LibraryDirs.Optick}/Win64-Debug"
			}

			postbuildcommands {
				"copy ThirdParty\\Optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-DebugEd"
			}
			
	filter { "configurations:Debug_Game" }
		defines {
			"AE_CONFIG_DEBUG_GAME"
		}

		links {
			"%{Libraries.Optick}"
		}

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Debug"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"%{LibraryDirs.Optick}/Win64-Debug"
			}

			postbuildcommands {
				"copy ThirdParty\\Optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Debug"
			}
			
	filter { "configurations:Release_Editor" }
		defines {
			"AE_CONFIG_RELEASE_EDITOR"
		}

		links {
			"%{Libraries.Optick}"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-ReleaseEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"%{LibraryDirs.Optick}/Win64-Release"
			}

			postbuildcommands {
				"copy ThirdParty\\Optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-ReleaseEd"
			}
			
	filter { "configurations:Release_Game" }
		defines {
			"AE_CONFIG_RELEASE_GAME"
		}

		links {
			"%{Libraries.Optick}"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Release"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"%{LibraryDirs.Optick}/Win64-Release"
			}

			postbuildcommands {
				"copy ThirdParty\\Optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Release"
			}
			
	filter { "configurations:Shipping_Game" }
		defines {
			"AE_CONFIG_SHIPPING_GAME"
		}

		links {
			
		}

		symbols "off"
		optimize "speed"

		filter { "configurations:Shipping_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Shipping"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				
			}

			postbuildcommands {
				
			}

	filter {}