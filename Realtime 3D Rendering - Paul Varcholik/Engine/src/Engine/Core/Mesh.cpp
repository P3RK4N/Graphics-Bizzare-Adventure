#include "pch.h"
#include "Mesh.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/ApplicationException.h"

#include <assimp/scene.h>

namespace Engine
{
	static HRESULT hr;

	Mesh::Mesh(Model& model, aiMesh& mesh)
		: m_Model(model)
	{
		m_Material = m_Model.getMaterials().at(mesh.mMaterialIndex);

		m_Vertices.reserve(mesh.mNumVertices);
		for(UINT i = 0; i < mesh.mNumVertices; i++)
			m_Vertices.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mVertices[i])));

		if(mesh.HasNormals())
		{
			m_Normals.reserve(mesh.mNumVertices);
			for(UINT i = 0; i < mesh.mNumVertices; i++)
				m_Normals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
		}

		if(mesh.HasTangentsAndBitangents())
		{
			m_Tangents.reserve(mesh.mNumVertices);
			m_BiNormals.reserve(mesh.mNumVertices);
			for(UINT i = 0; i < mesh.mNumVertices; i++)
			{
				m_Tangents.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
				m_BiNormals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
			}
		}

		UINT uvChannelCount = mesh.GetNumUVChannels();
		m_TextureCoordinates.reserve(uvChannelCount);
		for(UINT i = 0; i < uvChannelCount; i++)
		{
			std::vector<XMFLOAT3>* textureCoordinates = new std::vector<XMFLOAT3>();
			textureCoordinates->reserve(mesh.mNumVertices);
			m_TextureCoordinates.push_back(textureCoordinates);

			aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
			for(UINT j = 0; j < mesh.mNumVertices; j++)
				textureCoordinates->push_back(XMFLOAT3(reinterpret_cast<const float*>(&aiTextureCoordinates[j])));
		}

		UINT colorChannelCount = mesh.GetNumColorChannels();
		m_VertexColors.reserve(colorChannelCount);
		for(UINT i = 0; i < colorChannelCount; i++)
		{
			std::vector<XMFLOAT4>* vertexColors = new std::vector<XMFLOAT4>();
			vertexColors->reserve(mesh.mNumVertices);
			m_VertexColors.push_back(vertexColors);
			aiColor4D* aiVertexColors = mesh.mColors[i];
			for(UINT j = 0; j < mesh.mNumVertices; j++)
				vertexColors->push_back(XMFLOAT4(reinterpret_cast<const float*>(&aiVertexColors[j])));
		}

		if(mesh.HasFaces())
		{
			m_FaceCount = mesh.mNumFaces;
			for(UINT i = 0; i < m_FaceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for(UINT j = 0; j < face->mNumIndices; j++)
					m_Indices.push_back(face->mIndices[j]);
			}
		}

		if(mesh.HasBones())
		{
			m_BoneVertexWeights.resize(mesh.mNumVertices);

			for (UINT i = 0; i < mesh.mNumBones; i++)
			{
				aiBone* meshBone = mesh.mBones[i];

				UINT boneIndex = 0;
				std::string boneName = meshBone->mName.C_Str();
				auto boneMappingIterator = m_Model.m_BoneIndexMapping.find(boneName);
				if(boneMappingIterator != m_Model.m_BoneIndexMapping.end())
				{
					boneIndex = boneMappingIterator->second;
				}
				else
				{
					boneIndex = m_Model.m_Bones.size();
					XMFLOAT4X4 mat = XMFLOAT4X4(reinterpret_cast<const float*>(meshBone->mOffsetMatrix[0]));
					XMMATRIX offsetMatrix = XMLoadFloat4x4(&mat);
					XMFLOAT4X4 offset;
					XMStoreFloat4x4(&offset, XMMatrixTranspose(offsetMatrix));

					Bone* modelBone = new Bone(boneName, boneIndex, offset);
					m_Model.m_Bones.push_back(modelBone);
					m_Model.m_BoneIndexMapping[boneName] = boneIndex;
				}

				for(UINT j = 0; j < meshBone->mNumWeights; j++)
				{
					aiVertexWeight vertexWeight = meshBone->mWeights[j];
					m_BoneVertexWeights[vertexWeight.mVertexId].addWeight(vertexWeight.mWeight, boneIndex);
				}
			}
		}
	}

	Mesh::~Mesh()
	{
		for(std::vector<XMFLOAT3>* textureCoordinates : m_TextureCoordinates)
			delete textureCoordinates;

		for(std::vector<XMFLOAT4>* vertexColors : m_VertexColors)
			delete vertexColors;
	}

	void Mesh::createIndexBuffer(ID3D11Buffer** indexBuffer)
	{
		assert(indexBuffer != nullptr);

		D3D11_BUFFER_DESC indexBufferDesc =
		{
			sizeof(UINT) * m_Indices.size(),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		D3D11_SUBRESOURCE_DATA indexBufferData =
		{
			&m_Indices[0],
			0,
			0
		};

		if(FAILED(hr = m_Model.getApplication()->getDirect3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer)))
			throw ApplicationException("CreateBuffer() error", hr);
	}
}
