#pragma once
#include <string>
#include <vector>

namespace Engine
{
	class Application;
	class Mesh;
	class ModelMaterial;

	class Model
	{
	public:
		Model(Application* app, const std::string& fileName, bool flipUV = false);
		~Model();

		Application* getApplication() { return m_Application; }

		bool hasMeshes() const { return m_Meshes.size(); }
		bool hasMaterials() const { return m_Materials.size(); }

		const std::vector<Mesh*>& getMeshes() const { return m_Meshes; }
		const std::vector<ModelMaterial*>& getMaterials() const { return m_Materials; }

	private:
		Model (const Model&) = default;
		Model& operator=(const Model&) = default;

		Application* m_Application = nullptr;
		std::vector<Mesh*> m_Meshes{};
		std::vector<ModelMaterial*> m_Materials{};
	};
}
