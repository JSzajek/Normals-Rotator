workspace "NormalsRotator"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "NormalsRotator"

outputdir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Elysium/vendor/yaml-cpp"
	include "Elysium/vendor/imgui"
	include "Elysium/vendor/imguizmo"
	include "Elysium/vendor/imnodes"
group ""

group "Engine"
	include "Elysium"
group ""

group ""
	include "NormalsRotator"
group ""


