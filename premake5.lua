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
    externals["glm"]  = "external/glm"
    externals["assimp"] = "external/assimp"

    --Process externals before building our app
    include "external/glad"
    include "external/assimp"

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
            "%{prj.name}/include/**.cpp",
            "%{prj.name}/src/**.cpp",
        }

        sysincludedirs
        {
            "%{prj.name}/include",
            "%{externals.sdl2}/include",
            "%{externals.glad}/include",
            "%{externals.assimp}/include"
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

        links
        {
            "SDL2",
            "glad",
            "assimp"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }
