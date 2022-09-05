#include "PerkanTools.h"

const int width = 800;
const int height = 800;
glm::vec3 lightColor = glm::vec3(0.8f, 0.8f, 1.0f);
glm::vec3 lightWorldPos = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 cameraWorldPos = glm::vec3(0.0f, 0.5f, 4.0f);

int main()
{	
	//name,version,dimensions
	GLFWwindow* window = initialize("PerkanGL", 4, 6, width, height);

	gladLoadGL();
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	GLfloat background[] ={1.0f, 1.0f, 1.0f, 1.0f};
	//Camera-width,height,position,fov,near,far plane
	Camera cam(width, height, cameraWorldPos, 45.0f, 0.01f, 100.0f);


	//MODELS
	//----------------------------------------------------------------------------------------------
	// POSITION, NORMAL, COLOR, UV
	// FLOOR
	/*GLfloat a = 5.0f;
	std::vector<Vertex> floorVertices =
	{
		Vertex{glm::vec3(-a / 2, 0.0f, -a / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-a / 2, 0.0f, a / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(a / 2, 0.0f, a / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(a / 2, 0.0f, -a / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)},
	};
	std::vector<GLuint> floorIndices =
	{
		0, 1, 2,
		0, 2, 3
	};
	std::vector<Texture> floorTextures =
	{
		Texture("floor.png", DIFFUSE, GL_NEAREST, GL_REPEAT),
		Texture("floorSpecular.png", SPECULAR, GL_NEAREST, GL_REPEAT)
	};

	Mesh floor(floorVertices, floorIndices, floorTextures);

	Shader floorShader("floor1.vert", "floor.frag");

	glm::vec3 floorWorldPos = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorWorldPos);

	glUniformMatrix4fv(glGetUniformLocation(floorShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4fv(glGetUniformLocation(floorShader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(floorShader.ID, "lightWorldPos"), 1, glm::value_ptr(lightWorldPos));*/

	//----------------------------------------------------------------------------------------------
	//LIGHT
	GLfloat lightLength = 0.3f;
	glm::vec3 lightColor = glm::vec3(0.8f, 0.8f, 1.0f);
	std::vector<Vertex> lightVertices = 
	{			
		Vertex{glm::vec3(-lightLength / 2, -lightLength / 2, -lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(-lightLength / 2, lightLength / 2, -lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(lightLength / 2, -lightLength / 2, -lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(lightLength / 2, lightLength / 2, -lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(-lightLength / 2, -lightLength / 2, lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(-lightLength / 2, lightLength / 2, lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(lightLength / 2, -lightLength / 2, lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
		Vertex{glm::vec3(lightLength / 2, lightLength / 2, lightLength / 2), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)},
	};
	std::vector<GLuint> lightIndices =
	{
		0, 1, 2,
		1, 3, 2,

		0, 1, 4,
		1, 5, 4,

		2, 3, 6,
		3, 7, 6,

		4, 5, 6,
		5, 7, 6,

		1, 3, 5,
		3, 7, 5,

		0, 2, 4,
		2, 6, 4
	};
	std::vector<Texture> lightTextures;

	Mesh light(lightVertices, lightIndices, lightTextures);
	Shader lightShader("light.vert", "light.frag");

	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightWorldPos);
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
	//-----------------------------------------------------------------------------------------------
	// SWORD
	Model sword("Models/grindstone/scene.gltf");
	Shader swordShader("default.vert", "default.frag");
	glUniform4fv(glGetUniformLocation(swordShader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform4fv(glGetUniformLocation(swordShader.ID, "lightWorldPos"), 1, glm::value_ptr(lightWorldPos));
	//Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//Background color
		glClearColor(background[0],background[1],background[2],background[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Camera
		cam.Inputs(window);
		cam.UpdatePosition();
		
		//TMP
		light.Draw(lightShader, cam);
		//floor.Draw(floorShader, cam);
		sword.Draw(swordShader, cam);
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	//Close window, terminate process
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}