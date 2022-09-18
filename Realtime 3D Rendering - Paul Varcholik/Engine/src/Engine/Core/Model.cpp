#include "pch.h"
#include "Model.h"

#include "Mesh.h"
#include "ModelMaterial.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/ApplicationException.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Engine
{
	Model::Model(Application* app, const std::string& fileName, bool flipUV)
			: m_Application(app)
		{
			Assimp::Importer importer;
			UINT flags =
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_FlipWindingOrder;

			if(flipUV) flags |= aiProcess_FlipUVs;
			const aiScene* scene = importer.ReadFile(fileName, flags);

			if(!scene) throw ApplicationException("ReadFile() error");

			if(scene->HasMaterials())
				for(UINT i = 0; i < scene->mNumMaterials; i++)
					m_Materials.push_back(new ModelMaterial(*this, scene->mMaterials[i]));

			if(scene->HasMeshes())
				for(UINT i = 0; i < scene->mNumMeshes; i++)
					m_Meshes.push_back(new Mesh(*this, *(scene->mMeshes[i])));

			if(m_Bones.size())
				m_RootNode = buildSkeleton(*(scene->mRootNode), nullptr);

			if(scene->mNumAnimations)
			{
				for(UINT i = 0; i < scene->mNumAnimations; i++)
				{
					AnimationClip* animationClip = new AnimationClip(*this, *(scene->mAnimations[i]));
					m_AnimationClipsByName[animationClip->getName()] = animationClip;
				}
			}
		}

	Model::~Model()
	{
		for(Mesh* mesh : m_Meshes)
			delete mesh;

		for(ModelMaterial* material : m_Materials)
			delete material;
	}

	SceneNode* Model::buildSkeleton(aiNode& node, SceneNode* parentSceneNode)
	{
		SceneNode* sceneNode = nullptr;

		auto boneMapping = m_BoneIndexMapping.find(node.mName.C_Str());
		if(boneMapping == m_BoneIndexMapping.end())
		{
			sceneNode = new SceneNode(node.mName.C_Str());
		}
		else
		{
			sceneNode = m_Bones[boneMapping->second];
		}

		XMFLOAT4X4 mat = XMFLOAT4X4(reinterpret_cast<const float*>(node.mTransformation[0]));
		XMMATRIX transform = XMLoadFloat4x4(&mat);
		sceneNode->setTransform(XMMatrixTranspose(transform));
		sceneNode->setParent(parentSceneNode);

		for(UINT i = 0; i < node.mNumChildren; i++)
		{
			SceneNode* childSceneNode = buildSkeleton(*(node.mChildren[i]), sceneNode);
			sceneNode->getChildren().push_back(childSceneNode);
		}

		return sceneNode;
	}


}