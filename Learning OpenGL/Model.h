#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<unordered_map>

#include<json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
	public:
		Model(const char* file);
		void Draw(Shader& shader, Camera& camera);

	private:
		const char* file;
		std::vector<unsigned char> data;
		json JSON;

		std::vector<Mesh> meshes;
		std::vector<glm::vec3> translationsMeshes;
		std::vector<glm::quat> rotationsMeshes;
		std::vector<glm::vec3> scalesMeshes;
		std::vector<glm::mat4> matricesMeshes;

		std::unordered_map<std::string, unsigned int> loadedTexName;
		std::vector<Texture> loadedTextures;

		void getData();
		void getFloats(json accessor, std::vector<float>& floatVec);
		void getIndices(json accessor, std::vector<GLuint>& indices);
		void getTextures(std::vector<Texture>& textures);

		void assembleVertices
		(
			std::vector<Vertex>& vertices,
			std::vector<glm::vec3> position,
			std::vector<glm::vec3> normal,
			std::vector<glm::vec2> uv
		);

		void loadMesh(unsigned int indMesh);

		void traverseNode(unsigned int nextNode, glm::mat4 matrix);

		void groupFloatsVec2(std::vector<float>& floatVec, std::vector<glm::vec2>& vectors);
		void groupFloatsVec3(std::vector<float>& floatVec, std::vector<glm::vec3>& vectors);
		void groupFloatsVec4(std::vector<float>& floatVec, std::vector<glm::vec4>& vectors);
};

#endif