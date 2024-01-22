require("vstudio")

local vs = premake.vstudio.vc2010

local function premakeVersionComment(prj)
	premake.w('<!-- Generated by Premake ' .. _PREMAKE_VERSION .. ' -->')
end

local function vcpkg(prj)
	premake.w('<VcpkgTriplet Condition="\'$(Platform)\'==\'x64\'">x64-windows-static</VcpkgTriplet>')
	premake.w('<VcpkgEnabled>false</VcpkgEnabled>')
end

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
	local calls = base(prj)
	table.insertafter(calls, vs.xmlDeclaration, premakeVersionComment)
	return calls
end)

premake.override(premake.vstudio.vc2010.elements, "globals", function(base, prj)
	local calls = base(prj)
	table.insertafter(calls, vs.globals, vcpkg)
	return calls
end)

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
	include "Elysium/vendor/imgui_suite/imtexteditor"
group ""

group "Engine"
	include "Elysium"
group ""

group ""
	include "NormalsRotator"
group ""


