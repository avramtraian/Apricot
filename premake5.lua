workspace "Apricot"
	architecture "x64"
	configurations
	{
		"Editor_Debug",
		"Editor_Release",

		"Debug",
		"Release",
		"Shipping"
	}
	startproject "ApricotJam-Editor"

IncludeDirs = {}
IncludeDirs["ASTL"] = "%{wks.location}/Apricot/ThirdParty/ASTL/include"

group "Core"
	include "Apricot"
	include "Apricot/Runtime"
group "Tools"
	include "ApricotJam"
group ""