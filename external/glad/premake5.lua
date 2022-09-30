 project "glad"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "include/**.h",
        "src/**.c"
    }

    sysincludedirs
    {
        "include"
    }

    filter {"system:windows"}
        systemversion "latest"
        
        --Debug config generates redundant files with more info
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    --Release config generates only relevant files with optimized code
    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"