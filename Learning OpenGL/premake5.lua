project "Learning OpenGL"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   files
   {
      "./**.cpp",
      "./**.h"
   }

   includedirs
   {
      "./",
      includes.GLFW,
      includes.stb,
      includes.GLAD,
      includes.glm
   }

   links
   {
      "GLFW",
      "GLAD",
      "stb",
      "glm"
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