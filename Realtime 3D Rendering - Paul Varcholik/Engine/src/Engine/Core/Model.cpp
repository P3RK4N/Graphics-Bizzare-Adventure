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
		}

	Model::~Model()
	{
		for(Mesh* mesh : m_Meshes)
			delete mesh;

		for(ModelMaterial* material : m_Materials)
			delete material;
	}


}