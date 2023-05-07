include "Elysium/elysium_dependencies.lua"

project "NormalsRotator"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++17"

	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "norm_pch.h"
	pchsource "src/norm_pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"%{wks.location}/Elysium/src",
		"%{wks.location}/Elysium/vendor",

		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImNodes}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.IconFontCppHeaders}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}"
	}
	
	libdirs
	{
	}

	links
	{
		"Elysium",
	}

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