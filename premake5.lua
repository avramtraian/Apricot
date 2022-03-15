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

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDirs = {};
IncludeDirs["AE"] = "%{wks.location}/Apricot/Source"
IncludeDirs["VulkanSDK"] = "%{VULKAN_SDK}/Include";
IncludeDirs["Optick"] = "%{wks.location}/Apricot/ThirdParty/Optick/Include"

LibraryDirs = {};
LibraryDirs["VulkanSDK"] = "%{VULKAN_SDK}/Lib";
LibraryDirs["Optick"] = "%{wks.location}/Apricot/ThirdParty/optick/Binaries";

Libraries = {};
Libraries["Vulkan"] = "vulkan-1.lib"
Libraries["Optick"] = "OptickCore.lib"

group "Core"
    include "Apricot"
group "Tools"
    include "ApricotJam"
group "ThirdParty"
    
group ""