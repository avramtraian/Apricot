project "ApricotJam"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    filter { "system:windows", "configurations:Debug_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Debug")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter { "system:windows", "configurations:Release_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Release")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter { "system:windows", "configurations:Shipping_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Shipping")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter { "system:windows", "configurations:Debug_Editor" }

        targetdir ("%{wks.location}/Binaries/Win64-DebugEd")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter { "system:windows", "configurations:Release_Editor" }

        targetdir ("%{wks.location}/Binaries/Win64-ReleaseEd")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter {}

    pchheader "ajpch.h"
    pchsource "Source/ajpch.cpp"

    files
    {
        "Source/**.h",
        "Source/**.cpp",

        "resource.h",
        "ApricotJam.rc",
        "ApricotJam.aps"
    }

    includedirs
    {
        "%{IncludeDirs.AE}",
        "Source"
    }

    links
    {
        "AE"
    }

    defines
    {
        "AE_IMPORT_DLL"
    }

    filter { "system:windows" }
        systemversion "latest"
        defines
        {
            "AE_PLATFORM_WINDOWS"
        }

    filter { "configurations:Debug_Game" }
        defines
        {
            "AE_CONFIG_DEBUG_GAME"
        }

    filter { "configurations:Release_Game" }
        defines
        {
            "AE_CONFIG_RELEASE_GAME"
        }

    filter { "configurations:Shipping_Game" }
        defines
        {
            "AE_CONFIG_SHIPPING_GAME"
        }

    filter { "configurations:Debug_Editor" }
        defines
        {
            "AE_CONFIG_DEBUG_EDITOR"
        }

    filter { "configurations:Release_Editor" }
        defines
        {
            "AE_CONFIG_RELEASE_EDITOR"
        }