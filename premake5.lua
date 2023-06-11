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
	include "Elysium/vendor/glfw"
	include "Elysium/vendor/yaml-cpp"
	include "Elysium/vendor/imgui_suite/imgui"
	include "Elysium/vendor/imgui_suite/imguizmo"
	include "Elysium/vendor/imgui_suite/imnodes"
group ""

group "Engine"
	include "Elysium"
group ""

group ""
	include "NormalsRotator"
group ""


