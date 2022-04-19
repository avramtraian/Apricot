project "AE-Core"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	--#### GLOBAL - ANY PLATFORM ####--
	files
	{
		"Source/**.h",
		"Source/**.cpp",

		"ThirdParty/ASTL/include/**.h"
	}

	removefiles { "Source/Apricot/Platform/**.h", "Source/Apricot/Platform/**.cpp" }

	includedirs
	{
		"Source",

		"%{IncludeDirs.ASTL}"
	}

	pchheader "aepch.h"
	pchsource "Source/aepch.cpp"

	--#### ALL CONFIGURATIONS ####--
	filter { "system:windows" }
		files
		{
			"Source/Apricot/Platform/Windows/**.h",
			"Source/Apricot/Platform/Windows/**.cpp"
		}

	--#### EDITOR DEBUG ####--
	filter { "configurations:Editor_Debug" }
		
		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_EDITOR_DEBUG",
			"AE_EXPORT_CORE_LIBRARY"
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
			"AE_EXPORT_CORE_LIBRARY"
		}

		optimize "speed"

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
		defines
		{
			"AE_CONFIGURATION_DEBUG",
			"AE_EXPORT_CORE_LIBRARY"
		}

		optimize "off"

		--#### WINDOWS
		filter { "system:windows", "configurations:Debug" }
			debugdir  "%{wks.location}/Binaries/Win64/Debug"
			targetdir "%{wks.location}/Binaries/Win64/Debug"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			defines
			{
				"AE_PLATFORM_WINDOWS"
			}
	
	--#### RELEASE ####--
	filter { "configurations:Release" }

		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_RELEASE",
			"AE_EXPORT_CORE_LIBRARY"
		}

		optimize "speed"

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Release" }
			debugdir  "%{wks.location}/Binaries/Win64/Release"
			targetdir "%{wks.location}/Binaries/Win64/Release"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"

			defines
			{
				"AE_PLATFORM_WINDOWS"
			}
	
	--#### SHIPPING ####--
	filter { "configurations:Shipping" }

		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_SHIPPING",
			"AE_EXPORT_CORE_LIBRARY"
		}

		optimize "full"

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Shipping" }
			prebuildmessage ("Engine doesn't currently support building as a shipping product.")

			defines
			{
				"AE_PLATFORM_WINDOWS"
			}
		
	--#### GLOBAL ####--
	filter {}