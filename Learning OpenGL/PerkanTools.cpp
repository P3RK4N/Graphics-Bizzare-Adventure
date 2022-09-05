#include "PerkanTools.h"

GLFWwindow* initialize(const char* name, int major, int minor, int windowWidth, int windowHeight)
{
	
	//Initalize glfw
	glfwInit();
	//Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	//Set profile, core->modern, compatibility->deprecated
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Create GLFW window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
	//Just in case
	if (window == nullptr)
	{
		std::cout << "ERROR" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	//Introduce window to the current context? df is that
	glfwMakeContextCurrent(window);
	return window;
}