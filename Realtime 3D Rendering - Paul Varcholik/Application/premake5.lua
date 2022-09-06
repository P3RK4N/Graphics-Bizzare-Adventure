project "ApplicationDX11"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "%{wks.location}/Realtime 3D Rendering - Paul Varcholik/Engine/src",
        includes.spdlog,
        includes.DirectXTK,
        includes.FX11
    }

    links
    {
        "EngineDX11"
    }

    postbuildcommands
    {
        -- "copy %{wks.location}\Realtime 3D Rendering - Paul Varcholik\Resources\comicSansMS16.spritefont %{wks.location}\bin" .. '\' .. outputdir .. "\ApplicationDX11"
    }

    filter "system:windows"
            systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"