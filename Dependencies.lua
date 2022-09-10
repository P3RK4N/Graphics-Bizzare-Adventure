includes = {}
libraries = {}

includes["GLFW"] = "%{wks.location}/vendor/GLFW/include"
includes["spdlog"] = "%{wks.location}/vendor/spdlog/include"
includes["FX11"] = "%{wks.location}/vendor/FX11/inc"
includes["DirectXTK"] = "%{wks.location}/vendor/DirectXTK/Inc"
includes["stb"] = "%{wks.location}/vendor/stb"
includes["GLAD"] = "%{wks.location}/vendor/Glad/include"
includes["glm"] = "%{wks.location}/vendor/glm"
includes["assimp"] = "%{wks.location}/vendor/assimp/include"

libraries["FX11"] = "%{wks.location}/vendor/FX11/Bin/Desktop_2022_Win10/x64/%{cfg.buildcfg}/Effects11d.lib"
libraries["DirectXTK"] = "%{wks.location}/vendor/DirectXTK/Bin/Desktop_2022/x64/%{cfg.buildcfg}/DirectXTK.lib"
libraries["assimp"] = "%{wks.location}/vendor/assimp/lib/assimp-vc143-mtd.lib"