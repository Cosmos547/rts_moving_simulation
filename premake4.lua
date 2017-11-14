#!lua

includeDirList = {
    "SFML-2.4.2/include"
}

libDirectories = {
    "SFML-2.4.2/lib"
}

linkLibs = {
    "sfml-graphics",
    "sfml-window",
    "sfml-system"
}

buildOptions = {"-std=c++11"}

solution "CS488-final-project"
    configurations { "Debug", "Release" }

    project "rts"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "*.cpp", "*.h" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines  { "NDEBUG" }
        flags { "Optimize" }



