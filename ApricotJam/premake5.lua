project "ApricotJam"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

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

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "%{IncludeDirs.AE}"
    }

    links
    {
        "AE"
    }