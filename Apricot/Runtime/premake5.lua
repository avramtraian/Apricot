ExecutableName = "AE-Runtime"

project "AE-Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	targetname (ExecutableName)

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

	--#### EDITOR DEBUG ####--
	filter { "configurations:Editor_Debug" }
		
		--#### ANY PLATFORM ####--
		prebuildmessage ("Runtime is not a target for Editor_Debug configuration.")

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Editor_Debug" }
			debugdir  "%{wks.location}/Binaries/Win64/EdDebug"
			targetdir "%{wks.location}/Binaries/Win64/EdDebug"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
			
	--#### EDITOR RELEASE ####--
	filter { "configurations:Editor_Release" }
		
		--#### ANY PLATFORM ####--
		prebuildmessage ("Runtime is not a target for Editor_Release configuration.")

		--#### WINDOWS ####--
		filter { "system:windows", "configurations:Editor_Release" }
			debugdir  "%{wks.location}/Binaries/Win64/EdRelease"
			targetdir "%{wks.location}/Binaries/Win64/EdRelease"
			objdir    "%{wks.location}/Binaries-Int/Win64/%{prj.name}"
	
	--#### DEBUG ####--
	filter { "configurations:Debug" }

		--#### ANY PLATFORM ####--
		defines
		{
			"AE_CONFIGURATION_DEBUG",
			"AE_IMPORT_CORE_LIBRARY"
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
			"AE_IMPORT_CORE_LIBRARY"
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
			"AE_IMPORT_CORE_LIBRARY"
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