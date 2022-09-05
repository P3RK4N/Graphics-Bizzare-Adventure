#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
	public:
		glm::vec3 position;
		glm::vec3 orientation = glm::vec3(1.0f,0.0f,0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		int width;
		int height;

		bool firstClick = true;

		float speed = 0.05f;
		float sensitivity = 100.0f;

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		glm::mat4 camMatrix = glm::mat4(1.0f);

		Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);
		void Inputs(GLFWwindow* window);
		void UpdatePosition();
		void UpdateMatrix(Shader& shader, const char* uniform);
};

#endif
