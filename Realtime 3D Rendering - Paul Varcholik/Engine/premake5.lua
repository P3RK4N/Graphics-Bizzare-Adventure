project "EngineDX11"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "pch.h"
    pchsource "src/pch.cpp"
    
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        includes.FX11,
        includes.DirectXTK
    }

    links
    {
        libraries.FX11,
        libraries.DirectXTK,
        "D3D11.lib",
        "dinput8.lib",
        "dxguid.lib"
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