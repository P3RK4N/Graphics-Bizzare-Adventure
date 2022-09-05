project "spdlog"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   files
	{
      "include/**.cpp",
      "include/**.h",
      "src/**.cpp",
      "src/**.h"
   }

   includedirs
   {
      "include"
   }

   defines
   {
      "SPDLOG_COMPILED_LIB"
   }

   filter "system:windows"
      systemversion "latest"
   
   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"

   filter "configurations:Dist"
      runtime "Release"
      optimize "on"
      symbols "off"