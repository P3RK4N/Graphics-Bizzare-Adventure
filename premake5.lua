include "Dependencies.lua"

workspace "Graphics"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   flags { "MultiProcessorCompile" }

   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

   group "Dependencies"
      include "vendor/premake"
      include "vendor/GLFW"
      include "vendor/spdlog"
      include "vendor/Glad"
      include "vendor/stb"
      include "vendor/glm"
   group ""

   group "Projects"

      group "Learning OpenGL - Joey de Vries"
         include "Learning OpenGL"
      group ""

      group "Realtime 3D Rendering - Paul Varcholik"
         include "Realtime 3D Rendering - Paul Varcholik/Engine"
         include "Realtime 3D Rendering - Paul Varcholik/Application"
      group ""

   group ""