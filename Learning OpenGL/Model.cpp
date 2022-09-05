#include "Model.h"

Model::Model(const char* file)
{
	std::string text = get_file_contents(file);
	Model::JSON = json::parse(text);
	Model::file = file;

	getData();
	traverseNode(0, glm::mat4(1.0f));
}

void Model::Draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
}

void Model::getData()
{
	std::string bytesText;
	std::string uri = Model::JSON["buffers"][0]["uri"];
	std::string fileStr = std::string(Model::file);
	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDir + uri).c_str());
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	Model::data = data;
}

void Model::getFloats(json accessor, std::vector<float>& floatVec)
{
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = Model::JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Invalid type");

	unsigned int beginData = accByteOffset + byteOffset;
	unsigned int lengthData = count * 4 * numPerVert;

	for (unsigned int i = beginData; i < beginData + lengthData;)
	{
		unsigned char bytes[] = { Model::data[i++], Model::data[i++], Model::data[i++], Model::data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}
}

void Model::getIndices(json accessor, std::vector<GLuint>& indices)
{
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = Model::JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int beginData = byteOffset + accByteOffset;
	unsigned int lengthData = count * 4;

	if (componentType == 5125)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginData; i < beginData + lengthData;)
		{
			unsigned char bytes[] = { Model::data[i++], Model::data[i++],Model::data[i++],Model::data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}
}

void Model::getTextures(std::vector<Texture>& textures)
{
	std::string fileStr = std::string(file);
	std::string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];

		if (Model::loadedTexName.find(texPath) != Model::loadedTexName.end())
		{
			textures.push_back(Model::loadedTextures[Model::loadedTexName[texPath]]);
			continue;
		}
		if (texPath.find("baseColor") != std::string::npos)
		{
			Texture diffuse((fileDir + texPath).c_str(), DIFFUSE, GL_NEAREST, GL_REPEAT);
			textures.push_back(diffuse);
			Model::loadedTextures.push_back(diffuse);
			Model::loadedTexName[texPath] = Model::loadedTextures.size() - 1;
		}
		else if (texPath.find("metallicRoughness") != std::string::npos)
		{
			Texture specular = Texture((fileDir + texPath).c_str(), SPECULAR, GL_NEAREST, GL_REPEAT);
			textures.push_back(specular);
			Model::loadedTextures.push_back(specular);
			Model::loadedTexName[texPath] = Model::loadedTextures.size() - 1;
		}
	}
}

//3pos, 3norm, 3col(0 default), 2uv
void Model::assembleVertices(std::vector<Vertex>&  vertices, std::vector<glm::vec3> position, std::vector<glm::vec3> normal, std::vector<glm::vec2> uv)
{
	for (int i = 0; i < position.size(); i++)
	{
		vertices.push_back(Vertex{ position[i], normal[i], glm::vec3(0.0f), uv[i] });
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int uvAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	std::vector<float> posVec;
	std::vector<glm::vec3> positions;
	Model::getFloats(JSON["accessors"][posAccInd], posVec);
	Model::groupFloatsVec3(posVec, positions);

	std::vector<float> normalVec;
	std::vector<glm::vec3> normals;
	Model::getFloats(JSON["accessors"][normalAccInd], normalVec);
	Model::groupFloatsVec3(normalVec, normals);

	std::vector<float> uvVec;
	std::vector<glm::vec2> UVs;
	Model::getFloats(JSON["accessors"][uvAccInd], uvVec);
	Model::groupFloatsVec2(uvVec, UVs);

	std::vector<Vertex> vertices;
	Model::assembleVertices(vertices, positions, normals, UVs);
	std::vector<GLuint> indices;
	Model::getIndices(JSON["accessors"][indAccInd], indices);
	std::vector<Texture> textures;
	Model::getTextures(textures);

	Model::meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextnode, glm::mat4 matrix)
{
	json node = JSON["nodes"][nextnode];

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = node["translation"][i];
		translation = glm::make_vec3(transValues);
	}
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			//W value
			node["rotation"][3],
			//Y or X value
			node["rotation"][2],
			//X or Y value
			node["rotation"][1],
			//Z value
			node["rotation"][0]
		};
		rotation = glm::make_quat(rotValues);
	}
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = node["scale"][i];
		scale = glm::make_vec3(scaleValues);
	}
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = node["matrix"][i];
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

void Model::groupFloatsVec2(std::vector<float>& floatVec, std::vector<glm::vec2>& vectors)
{
	for (int i = 0; i < floatVec.size();)
	{
		glm::vec2 vector2;
		vector2.x = floatVec[i++];
		vector2.y = floatVec[i++];
		vectors.push_back(vector2);
	}
}

void Model::groupFloatsVec3(std::vector<float>& floatVec, std::vector<glm::vec3>& vectors)
{
	for (int i = 0; i < floatVec.size();)
	{
		glm::vec3 vector3;
		vector3.x = floatVec[i++];
		vector3.y = floatVec[i++];
		vector3.z = floatVec[i++];
		vectors.push_back(vector3);
	}
}

void Model::groupFloatsVec4(std::vector<float>& floatVec, std::vector<glm::vec4>& vectors)
{
	for (int i = 0; i < floatVec.size();)
	{
		glm::vec4 vector4;
		vector4.x = floatVec[i++];
		vector4.y = floatVec[i++];
		vector4.z = floatVec[i++];
		vector4.w = floatVec[i++];
		vectors.push_back(vector4);
	}
}