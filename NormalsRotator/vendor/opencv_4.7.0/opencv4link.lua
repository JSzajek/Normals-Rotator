function LinkOpenCV4()
	-- Get the location of the current script file
    local scriptLocation = debug.getinfo(1, "S").source:sub(2)

    -- Determine the relative directory based on the current script location
    local relativeDir = path.getdirectory(scriptLocation)

	includedirs
	{
		relativeDir .. "/include",
	}
	
	filter "configurations:Debug"
		links
		{
			"Iex-3_1_d.lib",
			"IlmThread-3_1_d.lib",
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
			"OpenEXR-3_1_d.lib",
			"OpenEXRCore-3_1_d.lib",
			"OpenEXRUtil-3_1_d.lib",
			"quirc.lib",
			"szip_D.lib",
			"tiffd.lib",
			"zlibd.lib"
		}
		libdirs
		{
			relativeDir .. "/Debug/lib"
		}
		postbuildcommands
		{
			"{COPY} " .. '"' .. relativeDir .. "/Debug/bin/" .. '"' .. " %{cfg.targetdir}"
		}
	filter "configurations:Release"
		links
		{
			"Iex-3_1.lib",
			"IlmThread-3_1.lib",
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
			"OpenEXR-3_1.lib",
			"OpenEXRCore-3_1.lib",
			"OpenEXRUtil-3_1.lib",
			"quirc.lib",
			"szip.lib",
			"tiff.lib",
			"zlib.lib"
		}
		libdirs
		{
			relativeDir .. "/Release/lib"
		}
		postbuildcommands
		{
			"{COPY} " .. '"' .. relativeDir .. "/Release/bin/" .. '"' .. " %{cfg.targetdir}"
		}
	filter "configurations:Dist"
		links
		{
			"Iex-3_1.lib",
			"IlmThread-3_1.lib",
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
			"OpenEXR-3_1.lib",
			"OpenEXRCore-3_1.lib",
			"OpenEXRUtil-3_1.lib",
			"quirc.lib",
			"szip.lib",
			"tiff.lib",
			"zlib.lib"
		}
		libdirs
		{
			relativeDir .. "/Release/lib"
		}
		postbuildcommands
		{
			"{COPY} " .. '"' .. relativeDir .. "/Release/bin/*.dll" .. '"' .. " %{cfg.targetdir}"
		}
end