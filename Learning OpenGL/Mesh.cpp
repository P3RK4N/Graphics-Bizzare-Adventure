#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	Mesh::vao.Bind();

	//Generates vertex buffer
	VBO vbo(vertices);
	//Generates indices buffer
	Mesh::ebo = EBO(indices);

	//Linking position
	Mesh::vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	//Linking normals
	Mesh::vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GL_FLOAT)));
	//Linking color
	Mesh::vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(GL_FLOAT)));
	//Linking texture uv
	Mesh::vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(GL_FLOAT)));


	Mesh::vao.Unbind();
	vbo.Unbind();
	Mesh::ebo.Unbind();
}

void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	shader.Activate();
	Mesh::vao.Bind();
	Mesh::ebo.Bind();

	//Bind textures to slots
	short numDiffuse = 0;
	short numSpecular = 0;
	for (int i = 0; i < Mesh::textures.size(); i++)
	{
		std::string num;
		std::string type = IMGTYPE_STR[Mesh::textures[i].imgType];

		if (Mesh::textures[i].imgType == DIFFUSE)
		{
			num = std::to_string(numDiffuse++);
		}
		else if (Mesh::textures[i].imgType == SPECULAR)
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].Activate(i, shader, (type + num).c_str());
	}
	
	glUniform3fv(glGetUniformLocation(shader.ID, "cameraWorldPos"), 1, glm::value_ptr(camera.position));
	camera.UpdateMatrix(shader, "camMatrix");

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale * glm::vec3(0.1f));

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(scal));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLES, Mesh::indices.size(), GL_UNSIGNED_INT, 0);
}
