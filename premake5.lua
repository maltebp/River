lib_directory = "lib/"

function static_libraries(libraries)
    for i, lib in ipairs(libraries) do
        local file_index = string.find(lib, "/[^/]*$")
        
        if file_index == nil then error() end

        local file = string.sub(lib, file_index+1)
        local directory = lib_directory .. string.sub(lib, 1, file_index)
        libdirs { directory }
        links { file }
        
        full_library_path = directory .. "/" .. file .. ".lib"
        -- Not sure if the relative path will be work on other platforms
        postbuildcommands { "{COPY} \"../" .. full_library_path .. "\" %{cfg.targetdir}" }
    end
end

workspace "River"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "build"
    cppdialect "C++17"


project "River"

    kind "StaticLib"
    language "C++"
    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"

    libdirs {
        lib_directory .. "OpenAL/bin/Release/x64"
    }

    links {
        "opengl32",
        "OpenAL32"
    }

    static_libraries {
        "glew/bin/%{cfg.buildcfg}/%{cfg.platform}/glew32s",
        "freetype/bin/%{cfg.buildcfg}/%{cfg.platform}/freetype",
        "RiverECS/bin/Debug/x64/RiverECS"
    }

    filter "action:gmake2"
        static_libraries {
            "glfw/lib-mingw-w64/glfw3",
        }

    filter "action:vs2017"
        static_libraries {
            "glfw/lib-vc2022/glfw3",
        }

    filter "action:vs2019"
        static_libraries {
            "glfw/lib-vc2022/glfw3",
        }

    filter "action:vs2022"
        static_libraries {
            "glfw/lib-vc2022/glfw3",
        }

    includedirs { 
        "src/River",

        -- Internalized library includes
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
    targetdir "%{wks.location}/bin/${prj.name}/%{cfg.buildcfg}"

    libdirs {
        "build/bin/River/%{cfg.buildcfg}"
    }
    
    links {
        "River",
        "glfw3",
        "glew32s",
        "freetype",
        "RiverECS"
    }

    files { "src/Sandbox/**.h", "src/Sandbox/**.cpp", "src/Sandbox/**.hpp" }
    
    includedirs { 
        "src/Sandbox",
        "src/River"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"
