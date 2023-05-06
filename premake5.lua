workspace "NormalsRotator"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	platforms { "x64" }

	startproject "NormalsRotator"

outputdir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}"

project "Elysium"
	location "Elysium"
	kind "StaticLib"

	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "elysium_pch.h"
	pchsource "%{prj.name}/src/elysium_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**",

		"%{prj.name}/vendor/imgui/**",
		"%{prj.name}/vendor/imguizmo/**",
		"%{prj.name}/vendor/imnodes/**",
	}

	includedirs
	{
		"%{prj.name}/src",

		"%{prj.name}/vendor/stb_image",

		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/imguizmo",
		"%{prj.name}/vendor/imnodes",
		"%{prj.name}/vendor/iconfontcppheaders",

		"%{prj.name}/vendor/entt",
		"%{prj.name}/vendor/stduuid",
	}
	
	libdirs
	{
	}
	
	links
	{
	}

	postbuildcommands {
		"{COPY} Content/ %{cfg.targetdir}/Content/"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"PROJDIR=\'$(ProjectDir).\'",
			"SOLUDIR=\'$(SolutionDir).\'"
		}

	filter "configurations:Debug"
		defines "ELYSIUM_DEBUG"
		symbols "on"
	filter "configurations:Release"
		defines "ELYSIUM_RELEASE"
		optimize "on"
	filter "configurations:Dist"
		defines "ELYSIUM_DIST"
		optimize "Full"

project "NormalsRotator"
	location "NormalsRotator"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++17"

	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "norm_pch.h"
	pchsource "%{prj.name}/src/norm_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",

		"Elysium/src",
		"Elysium/vendor/imgui",
		"Elysium/vendor/imguizmo",
		"Elysium/vendor/imnodes",
		"Elysium/vendor/iconfontcppheaders",

		"Elysium/vendor/entt",
		"Elysium/vendor/stduuid",
	}
	
	libdirs
	{
	}

	links
	{
		"Elysium",
	}

	postbuildcommands {
		"{COPY} Content/ %{cfg.targetdir}/Content/"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"PROJDIR=\'$(ProjectDir).\'",
			"SOLUDIR=\'$(SolutionDir).\'"
		}

	filter "configurations:Debug"
		defines "ELYSIUM_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "ELYSIUM_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "ELYSIUM_DIST"
		optimize "Full"
