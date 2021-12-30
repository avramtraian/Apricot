project "ApricotJam"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	pchheader "ajpch.h"
	pchsource "Source/ajpch.cpp"

	files {
		"Source/**.h",
		"Source/**.cpp",

		"resource.h",
		"ApricotJam.rc",
		"ApricotJam.aps"
	}

	includedirs {
		"Source",

		"%{IncludeDirs.AE}",
		"%{IncludeDirs.Optick}"
	}

	links {
		"AE"
	}

	defines {
		"AE_IMPORT_DLL"
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

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-DebugEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	filter { "configurations:Debug_Game" }
		defines {
			"AE_CONFIG_DEBUG_GAME"
		}

		symbols "on"
		optimize "off"

		filter { "configurations:Debug_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Debug"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	filter { "configurations:Release_Editor" }
		defines {
			"AE_CONFIG_RELEASE_EDITOR"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Editor", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-ReleaseEd"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	filter { "configurations:Release_Game" }
		defines {
			"AE_CONFIG_RELEASE_GAME"
		}

		symbols "off"
		optimize "full"

		filter { "configurations:Release_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Release"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	filter { "configurations:Shipping_Game" }
		defines {
			"AE_CONFIG_SHIPPING_GAME"
		}

		symbols "off"
		optimize "speed"

		filter { "configurations:Shipping_Game", "system:windows" }
			targetdir "%{wks.location}/Binaries/Win64-Shipping"
			objdir "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	filter {}