#pragma once
#include "Bone.h"
#include "AnimationClip.h"

#include <string>
#include <vector>

#include <assimp/mesh.h>

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

		std::unordered_map<std::string, AnimationClip*>& getAnimationClipsByName() { return m_AnimationClipsByName; }
		SceneNode* getRootNode() { return m_RootNode; }
		std::vector<Bone*>& getBones() { return m_Bones; }

		SceneNode* buildSkeleton(aiNode& node, SceneNode* parentSceneNode);

	private:
		Model (const Model&) = default;
		Model& operator=(const Model&) = default;

		Application* m_Application = nullptr;
		std::vector<Mesh*> m_Meshes{};
		std::vector<ModelMaterial*> m_Materials{};
		std::unordered_map<std::string, AnimationClip*> m_AnimationClipsByName{};

		std::vector<Bone*> m_Bones{};
		std::unordered_map<std::string, UINT> m_BoneIndexMapping{};
		SceneNode* m_RootNode{};

		friend class Mesh;
		friend class BoneAnimation;
	};
}
