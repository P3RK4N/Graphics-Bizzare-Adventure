#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
{
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;
	Camera::proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
}

void Camera::UpdatePosition()
{
	view = lookAt(position, position + orientation, up);
	camMatrix = proj * view;
}

void Camera::UpdateMatrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, value_ptr(camMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= speed * normalize(cross(orientation,up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * normalize(cross(orientation,up));
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}
		
		double mousex;
		double mousey;
		glfwGetCursorPos(window, &mousex, &mousey);

		float rotX = sensitivity * (float)(mousey - (height / 2)) / height;
		float rotY = sensitivity * (float)(mousex - (height / 2)) / height;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), normalize(cross(orientation, up)));
		if (!(angle(newOrientation, up) <= glm::radians(5.0f) or angle(newOrientation, -up) <= glm::radians(5.0f)))
		{
			orientation = newOrientation;
		}

		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}