workspace "rendeer"
    startproject "rendeer"

    configurations
    {
        "Debug",
        "Release"
    }

    --Target and object directories
    tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
    odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

    --External Dependencies
    externals = {}

    externals["sdl2"] = "external/sdl2"
    externals["glad"] = "external/glad"

    --Process Glad before building our app
    include "external/glad"

    --Main editor application
    project "Rendeer"
        location "Rendeer"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir(tdir)
        objdir(odir)

        files 
        {
            "%{prj.name}/include/**.h",
            "%{prj.name}/src/**.cpp",
        }

        sysincludedirs
        {
            "%{prj.name}/include",
            "%{externals.sdl2}/include",
            "%{externals.glad}/include"
        }

        flags
        {
            "FatalWarnings"
        }
        
        defines
        { 
            "GLFW_INCLUDE_NONE", --Ensures that Glad doesn't include GLFW
            "RENDEER_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2",
            "glad"
        }