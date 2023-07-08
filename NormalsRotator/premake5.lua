include "Elysium/elysiumlink.lua"
include "Elysium/vendor/imgui_suite/imgui_dependencies.lua"
include "vendor/opencv_4.6.0/opencv4link.lua"

project "NormalsRotator"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++17"

	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "norm_pch.h"
	pchsource "src/norm_pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"../Content/**"
	}

	includedirs
	{
		"src",

		"%{ImGui_IncludeDir.ImGui}",
		"%{ImGui_IncludeDir.ImGuizmo}",
		"%{ImGui_IncludeDir.ImNodes}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.IconFontCppHeaders}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.spd_log}"
	}

	postbuildcommands
	{
		"{COPY} " .. '"' .. "%{wks.location}/NormalsRotator/imgui.ini" .. '"' .. " %{cfg.targetdir}"
	}

	LinkElysium()
	LinkOpenCV4()

	filter "system:windows"
		systemversion "latest"
	filter "configurations:Debug"
		defines "ELYSIUM_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "ELYSIUM_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "ELYSIUM_DIST"
		optimize "Full"