workspace "Cyclope"
    architecture "x64"
    startproject "App"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ImGui"
    location "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "$(SolutionDir)ImGui/include"
    }

    libdirs
    {
        "$(SolutionDir)OpenGL/src",
    }

    links
    {
        "glfw3.lib"
    }

    defines 
    { 
        "IMGUI_API=__declspec(dllexport)" 
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

project "Cyclope"
    location "Cyclope"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Cpch.h"
    pchsource "Cyclope/src/Cpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "$(SolutionDir)OpenGL/include",
        "$(SolutionDir)vendor/glm",
        "$(SolutionDir)vendor/entt",
        "$(SolutionDir)vendor/stb_image",
        "$(SolutionDir)vendor/spdlog",
        "$(SolutionDir)vendor/ImGuizmo",
        "$(SolutionDir)yaml-cpp/include",
        "$(SolutionDir)ImGui/include",
        "Cyclope/src/Cyclope",
        "Cyclope/src"
    }
    
    libdirs
    {
        "$(SolutionDir)OpenGL/src",
    }

    links
    {
        "glfw3.lib",
        "ImGui",
        "OpenGL",
        "yaml-cpp"
    }

    filter "fileS:vendor/ImGuizmo/**.cpp"
    flags {"NoPCH"}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CYCLOPE_BUILD_DLL",
        }

        postbuildcommands
        {
            ("{COPY} ../bin/" .. outputdir .. "/Cyclope/Cyclope.dll ../bin/" .. outputdir .. "/App")
        }

    filter "configurations:Debug"
        defines {"CYCLOPE_DEBUG", "CYCLOPE_ENABLE_ASSERTS", "CYCLOPE_PROFILE"}
        symbols "On"
        buildoptions "/MD"

    filter "configurations:Release"
        defines "CYCLOPE_RELEASE"
        optimize "On"
        buildoptions "/MD"

project "Scripting"
    location "Scripting"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.c"
    }

    includedirs
    {
        "$(SolutionDir)OpenGL/include",
        "$(SolutionDir)vendor/glm",
        "$(SolutionDir)vendor/entt",
        "$(SolutionDir)vendor/spdlog",
        "$(SolutionDir)ImGui/include",
        "Cyclope/src",
        "Cyclope/src/Cyclope",
        "Cyclope/src/NativeScripting",
        "$(SolutionDir)Scripting/Components",
        "$(SolutionDir)Scripting"
    }
    
    libdirs
    {
        "$(SolutionDir)OpenGL/src",
    }

    links
    {
        "glfw3.lib",
        "ImGui",
        "Cyclope"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            
        }

        postbuildcommands
        {
            ("{COPY} ../bin/" .. outputdir .. "/Scripting/Scripting.dll ../bin/" .. outputdir .. "/App")
        }

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
        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "$(SolutionDir)OpenGL/include",
        "$(SolutionDir)vendor/glm",
        "$(SolutionDir)vendor/entt",
        "$(SolutionDir)vendor/spdlog",
        "$(SolutionDir)vendor/ImGuizmo",
        "$(SolutionDir)ImGui/include",
        "Cyclope/src",
        "Cyclope/src/Cyclope"
    }

    libdirs
    {
        "$(SolutionDir)OpenGL/src",
    }

    links
    {
        "glfw3.lib",
        "Cyclope",
        "ImGui",
        "Scripting"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        buildoptions "/MT"

    filter "configurations:Release"
        optimize "On"
        buildoptions "/MT"

project "OpenGL"
        location "OpenGL"
        kind "StaticLib"
        language "C++"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp",
            "%{prj.name}/**.c",
            "%{prj.name}/**.lib"
        }
    
        includedirs
        {
            "$(SolutionDir)OpenGL/include"
        }
    
        libdirs
        {
            
        }
    
        links
        {
            
        }
    
        defines 
        { 
            
        }
    
        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

project "yaml-cpp"
        location "yaml-cpp"
        kind "StaticLib"
        language "C++"
    
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "src/**.h",
            "src/**.cpp",
            
            "include/**.h"
        }
    
        includedirs
        {
            "$(SolutionDir)yaml-cpp/include"
        }
    
        defines
        {
            "YAML_CPP_STATIC_DEFINE"
        }
    
        filter "system:windows"
            systemversion "latest"
            cppdialect "C++17"
            staticruntime "off"
    
        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            runtime "Release"
            optimize "on"