workspace "Apricot"
    configurations
    {
        "Debug_Game",
        "Release_Game",
        "Shipping_Game",

        "Debug_Editor",
        "Release_Editor"
    }

    architecture "x64"
    startproject "ApricotJam"

IncludeDirs = {};
IncludeDirs["AE"] = "%{wks.location}/Apricot/Source"

group "Core"
    include "Apricot"
group "Tools"
    include "ApricotJam"
group "Dependencies"

group ""