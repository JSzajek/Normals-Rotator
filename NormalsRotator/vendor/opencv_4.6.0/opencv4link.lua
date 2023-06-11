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
			"opencv_world460d.lib"
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
			"opencv_world460.lib"
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
			"opencv_world460.lib"
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