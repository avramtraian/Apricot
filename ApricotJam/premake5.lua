project "ApricotJam-Editor"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	--#### GLOBAL - ANY PLATFORM #
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp",
		"Source/**.inl"
	}

	includedirs
	{
		"Source",
		"%{wks.location}/Apricot/Source",

		"%{IncludeDirs.ASTL}"
	}

	links
	{
		"AE-Core"
	}

	pchheader "ajpch.h"
	pchsource "Source/ajpch.cpp"

	--#### EDITOR DEBUG ####--
	filter { "configurations:Editor_Debug" }
		
		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_EDITOR_DEBUG",
			"AE_IMPORT_CORE_LIBRARY"
		}

		optimize "off"

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Editor_Debug" }
			debugdir  "%{wks.location}/Binaries/Win64/EdDebug"
			targetdir "%{wks.location}/Binaries/Win64/EdDebug"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			defines
			{
				"AE_PLATFORM_WINDOWS"
			}
			
	--#### EDITOR RELEASE ####--
	filter { "configurations:Editor_Release" }
		
		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_EDITOR_RELEASE",
			"AE_IMPORT_CORE_LIBRARY"
		}

		optimize "Speed"

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Editor_Release" }
			debugdir  "%{wks.location}/Binaries/Win64/EdRelease"
			targetdir "%{wks.location}/Binaries/Win64/EdRelease"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			defines
			{
				"AE_PLATFORM_WINDOWS"
			}
	
	--#### DEBUG ####--
	filter { "configurations:Debug" }

		--#### ANY PLATFORM ####--
		prebuildmessage ("Editor is not a target for Debug configuration.")

		--#### WINDOWS
		filter { "system:windows", "configurations:Debug" }
			debugdir  "%{wks.location}/Binaries/Win64/Debug"
			targetdir "%{wks.location}/Binaries/Win64/Debug"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
	
	--#### RELEASE ####--
	filter { "configurations:Release" }

		--#### ANY PLATFORM ####--
		prebuildmessage ("Editor is not a target for Release configuration.")

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Release" }
			debugdir  "%{wks.location}/Binaries/Win64/Release"
			targetdir "%{wks.location}/Binaries/Win64/Release"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
	
	--#### SHIPPING ####--
	filter { "configurations:Shipping" }

		--#### ANY PLATFORM ####--
		prebuildmessage ("Editor is not a target for Shipping configuration.")

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Shipping" }
			prebuildmessage ("Engine doesn't currently support building as a shipping product.")
		
	--#### GLOBAL ####--
	filter {}