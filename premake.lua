workspace "Cyclope"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Cyclope"
    location "Cyclope"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    include
    {
        "%{prj.name}/vendor/OpenGL/include"
        "%{prj.name}/vendor/glm"
        "%{prj.name}/vendor/entt"
    } 

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CYCLOPE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %(cfg.buildtarget.relpath) ../bin/" .. outputdir .. "/App")
        }

    filter "configurations:Debug":
        defines "CYCLOPE_DEBUG"
        symbols "On"

    filter "configurations:Release":
        defines "CYCLOPE_RELEASE"
        optimize "On"

project "App"
    location "App"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    include
    {
        "%{prj.name}/vendor/OpenGL/include"
        "%{prj.name}/vendor/glm"
        "%{prj.name}/vendor/entt"
        "Cyclope/src"
    }

    links
    {
        "Cyclope"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"