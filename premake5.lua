workspace "PluginDemo"
    architecture "x64"
    startproject "core"

    configurations { "Debug", "Release" }

    filter "system:windows"
        systemversion "latest"
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    filter {}

----------------------------------------------------
-- Core host executable
----------------------------------------------------
project "core"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "core/**.h",
        "core/**.cpp"
    }

    includedirs {
        "core"
    }

----------------------------------------------------
-- Plugin: PluginAdd (Shared Library / DLL)
----------------------------------------------------
project "PluginAdd"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "PluginAdd/**.h",
        "PluginAdd/**.cpp"
    }

    includedirs {
        "core"   -- for PluginAPI.h
    }

----------------------------------------------------
-- Plugin: PluginMul (Shared Library / DLL)
----------------------------------------------------
project "PluginMul"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "PluginMul/**.h",
        "PluginMul/**.cpp"
    }

    includedirs {
        "core"   -- for PluginAPI.h
    }
