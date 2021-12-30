project "AE"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    pchheader "aepch.h"
    pchsource "Source/aepch.cpp"

    files
    {
        "Source/**.h",
        "Source/**.cpp",

        "Vendor/optick/Include/**.h"
    }

    includedirs
    {
        "Source",

        "%{IncludeDirs.optick}"
    }

    links
    {
        "OptickCore.lib"
    }

    defines
    {
        "AE_EXPORT_DLL"
    }

    filter { "configurations:Debug_Game or configurations:Debug_Editor" }
        libdirs
        {
            "Vendor/optick/Binaries/Win64-Debug"
        }
        
    filter { "configurations:Release_Game or Release_Editor" }
        libdirs
        {
            "Vendor/optick/Binaries/Win64-Release"
        }
        
    filter {}
        
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
        postbuildcommands
        {

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

    filter { "system:windows", "configurations:Debug_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Debug")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

        postbuildcommands
        {
            "copy Vendor\\optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Debug"
        }

    filter { "system:windows", "configurations:Release_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Release")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

        postbuildcommands
        {
            "copy Vendor\\optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-Release"
        }

    filter { "system:windows", "configurations:Shipping_Game" }

        targetdir ("%{wks.location}/Binaries/Win64-Shipping")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

    filter { "system:windows", "configurations:Debug_Editor" }

        targetdir ("%{wks.location}/Binaries/Win64-DebugEd")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

        postbuildcommands
        {
            "copy Vendor\\optick\\Binaries\\Win64-Debug\\OptickCore.dll %{wks.location}\\Binaries\\Win64-DebugEd"
        }

    filter { "system:windows", "configurations:Release_Editor" }

        targetdir ("%{wks.location}/Binaries/Win64-ReleaseEd")
        objdir ("%{wks.location}/Binaries-Int/%{prj.name}")

        postbuildcommands
        {
            "copy Vendor\\optick\\Binaries\\Win64-Release\\OptickCore.dll %{wks.location}\\Binaries\\Win64-ReleaseEd"
        }

    filter {}