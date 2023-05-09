include "Elysium/elysium_dependencies.lua"
include "Elysium/vendor/imgui_suite/imgui_dependencies.lua"

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
	}

	includedirs
	{
		"src",
		"%{wks.location}/Elysium/src",
		"%{wks.location}/Elysium/vendor",

		"vendor/opencv_4.6.0/include",

		"%{ImGui_IncludeDir.ImGui}",
		"%{ImGui_IncludeDir.ImGuizmo}",
		"%{ImGui_IncludeDir.ImNodes}",
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
		"Elysium"
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

	filter "configurations:Release"
		links
		{
			"jpeg.lib",
			"libpng16.lib",
			"libprotobuf.lib",
			"libprotobuf-lite.lib",
			"libprotoc.lib",
			"lzma.lib",
			"opencv_calib3d4.lib",
			"opencv_core4.lib",
			"opencv_dnn4.lib",
			"opencv_features2d4.lib",
			"opencv_flann4.lib",
			"opencv_highgui4.lib",
			"opencv_imgcodecs4.lib",
			"opencv_imgproc4.lib",
			"opencv_ml4.lib",
			"opencv_objdetect4.lib",
			"opencv_photo4.lib",
			"opencv_stitching4.lib",
			"opencv_video4.lib",
			"opencv_videoio4.lib",
			"quirc.lib",
			"tiff.lib",
			"turbojpeg.lib",
			"webp.lib",
			"webpdecoder.lib",
			"webpdemux.lib",
			"webpmux.lib",
			"zlib.lib"
		}
		libdirs
		{
			"vendor/opencv_4.6.0/Release/lib"
		}
		postbuildcommands
		{
			"{COPY} \"%{prj.location}/vendor/opencv_4.6.0/Release/bin\" %{cfg.targetdir}"
		}

	filter "configurations:Debug"
		links
		{
			"jpeg.lib",
			"libpng16d.lib",
			"libprotobufd.lib",
			"libprotobuf-lited.lib",
			"libprotocd.lib",
			"lzma.lib",
			"opencv_calib3d4d.lib",
			"opencv_core4d.lib",
			"opencv_dnn4d.lib",
			"opencv_features2d4d.lib",
			"opencv_flann4d.lib",
			"opencv_highgui4d.lib",
			"opencv_imgcodecs4d.lib",
			"opencv_imgproc4d.lib",
			"opencv_ml4d.lib",
			"opencv_objdetect4d.lib",
			"opencv_photo4d.lib",
			"opencv_stitching4d.lib",
			"opencv_video4d.lib",
			"opencv_videoio4d.lib",
			"quirc.lib",
			"tiffd.lib",
			"turbojpeg.lib",
			"webp.lib",
			"webpdecoder.lib",
			"webpdemux.lib",
			"webpmux.lib",
			"zlibd.lib"
		}
		libdirs
		{
			"vendor/opencv_4.6.0/Debug/lib"
		}
		postbuildcommands
		{
			"{COPY} \"%{prj.location}/vendor/opencv_4.6.0/Debug/bin\" %{cfg.targetdir}"
		}