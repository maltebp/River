workspace "River"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "build"
    cppdialect "C++17"


project "River"

    kind "StaticLib"
    language "C++"
    targetdir "build/bin/River/%{cfg.buildcfg}"

    libdirs {
        "lib/glfw/bin/Release/x64",
        "lib/glew/bin/%{cfg.buildcfg}/%{cfg.platform}",
        "lib/OpenAL/bin/Release/x64",
        "lib/freetype/bin/%{cfg.buildcfg}/%{cfg.platform}",
        "lib/RiverECS/bin/Debug/x64"
    }

    links {
        "opengl32",
        "glfw3",
        "glew32s",
        "OpenAL32",
        "freetype",
        "RiverECS"
    }

    includedirs { 
        "src/River",
        "lib/glew/include",
        "lib/glfw/include",
        "lib/glm/include",
        "lib/OpenAL/include",
        "lib/stb_image/include",
        "lib/freetype/include",
        "lib/dr_wav/include"
    }

    files { "src/River/**.h", "src/River/**.cpp", "src/River/**.hpp" }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"

project "Sandbox"

    kind "ConsoleApp" -- Distinction from a "windowed app" is important apparently
    language "C++"
    targetdir "build/bin/Sandbox/%{cfg.buildcfg}"

    libdirs {
        "lib/glfw/bin/Release/x64",
        "lib/glew/bin/%{cfg.buildcfg}/%{cfg.platform}",
        "lib/OpenAL/bin/Release/x64",
        "lib/freetype/bin/%{cfg.buildcfg}/%{cfg.platform}",
        "lib/RiverECS/bin/Debug/x64"
    }

    links {
        "River",
        "opengl32",
        "glfw3",
        "glew32s",
        "OpenAL32",
        "freetype",
        "RiverECS"
    }

    files { "src/Sandbox/**.h", "src/Sandbox/**.cpp", "src/Sandbox/**.hpp" }
    
    includedirs { 
        "src/Sandbox",
        "src/River",
        "lib/glew/include",
        "lib/glm/include", -- TODO: This should be removed
        "lib/glfw/include" -- TODO: This should be removed
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"
