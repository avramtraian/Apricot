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

		"Vendor/Optick/Include/**.h"
	}

	includedirs {
		"Source",

		"%{IncludeDirs.Optick}"
	}

	defines {
		"AE_EXPORT_DLL"
	}

	filter { "system:windows" }
		systemversion "latest"
		defines {
			"AE_PLATFORM_WINDOWS"
		}

	filter { "configurations:Debug_Editor" }
		defines {
			"AE_CONFIG_DEBUG_EDITOR"
		}

		links {
			"OptickCore.lib"
		}

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-DebugEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"Vendor/Optick/Binaries/Win64-Debug"
			}

			postbuildcommands {
				"copy Vendor\\Optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-DebugEd"
			}
			
	filter { "configurations:Debug_Game" }
		defines {
			"AE_CONFIG_DEBUG_GAME"
		}

		links {
			"OptickCore.lib"
		}

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Debug"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"Vendor/Optick/Binaries/Win64-Debug"
			}

			postbuildcommands {
				"copy Vendor\\Optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Debug"
			}
			
	filter { "configurations:Release_Editor" }
		defines {
			"AE_CONFIG_RELEASE_EDITOR"
		}

		links {
			"OptickCore.lib"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-ReleaseEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"Vendor/Optick/Binaries/Win64-Release"
			}

			postbuildcommands {
				"copy Vendor\\Optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-ReleaseEd"
			}
			
	filter { "configurations:Release_Game" }
		defines {
			"AE_CONFIG_RELEASE_GAME"
		}

		links {
			"OptickCore.lib"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Release"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			libdirs {
				"Vendor/Optick/Binaries/Win64-Release"
			}

			postbuildcommands {
				"copy Vendor\\Optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Release"
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