rv = {}

rv.engine.core = {}

rv.engine.core.links

src_directory = river_dir .. "/src/"
lib_directory = river_dir .. "/lib/"

river_links = { }

function static_libraries(libraries)
    for i, lib in ipairs(libraries) do
        local file_index = string.find(lib, "/[^/]*$")
        
        if file_index == nil then error() end

        local file = string.sub(lib, file_index+1)
        local directory = lib_directory .. string.sub(lib, 1, file_index)
        libdirs { directory }
        links { file }

        table.insert(river_links, file)
        
        full_library_path = directory .. "/" .. file .. ".lib"

        prebuildcommands {
            '{DELETE} "%{cfg.targetdir}/' .. file .. '.lib"'
        }

        postbuildcommands { 
            '{COPYFILE} "' .. full_library_path .. '" "%{cfg.targetdir}"'
        }
    end
end


project "river.engine.core"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    platforms { "x64" }
    configurations { "Debug", "Release" }

    location(river_dir .. "/build/River")
    objdir "%{prj.location}/obj/%{cfg.buildcfg}"
    targetdir(river_dir .. "build/River/bin/%{cfg.buildcfg}")

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
        river_dir .. "/src/River",

        -- Internalized library includes
        lib_directory .. "/OpenAL/include",
        lib_directory .. "/stb_image/include",
        lib_directory .. "/freetype/include",
        lib_directory .. "/dr_wav/include"
    }

    files { 
        river_dir .. "/src/River/**.h",
        river_dir .. "/src/River/**.cpp",
        river_dir .. "/src/River/**.hpp"
    }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"

    prebuildcommands {
        '{RMDIR} "%{cfg.targetdir}/include"'
    }

    pchheader("River/pch.h")
    pchsource(_SCRIPT_DIR .. "/River/pch.cpp")