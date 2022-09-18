#include "pch.h"
#include "AnimationMaterial.h"
#include "Engine/Core/BoneVertexWeight.h"
#include "Engine/Core/Mesh.h"

#include "Engine/Core/Application.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(AnimationMaterial)

	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, WorldViewProjection)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, World)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, CameraPosition)

	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, AmbientColor)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, SpecularColor)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, SpecularPower)

	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, LightColor)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, LightPosition)
	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, LightRadius)

	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, ColorTexture)

	MATERIAL_VARIABLE_DEFINITION(AnimationMaterial, BoneTransforms)


	void AnimationMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
		MATERIAL_VARIABLE_RETRIEVE(World)
		MATERIAL_VARIABLE_RETRIEVE(CameraPosition)

		MATERIAL_VARIABLE_RETRIEVE(AmbientColor)
		MATERIAL_VARIABLE_RETRIEVE(SpecularColor)
		MATERIAL_VARIABLE_RETRIEVE(SpecularPower)

		MATERIAL_VARIABLE_RETRIEVE(LightColor)
		MATERIAL_VARIABLE_RETRIEVE(LightPosition)
		MATERIAL_VARIABLE_RETRIEVE(LightRadius)

		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)

		MATERIAL_VARIABLE_RETRIEVE(BoneTransforms)

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("main11", "p0", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

	void AnimationMaterial::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		const std::vector<XMFLOAT3>& sourceUV = *mesh.getTextureCoordinates().at(0);
		const std::vector<XMFLOAT3>& sourceNormals = mesh.getNormals();
		const std::vector<BoneVertexWeights>& sourceWeights = mesh.getBoneVertexWeights();


		std::vector<AnimationMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());

		for(UINT i = 0; i < sourceVertices.size(); i++)
		{
			const auto bvw = sourceWeights[i].getWeights();
			UINT s = bvw.size();
			XMFLOAT4 weights = 
			{ 
				s > 0 ? bvw[0].weight : 0, 
				s > 1 ? bvw[1].weight : 0, 
				s > 2 ? bvw[2].weight : 0, 
				s > 3 ? bvw[3].weight : 0 
			};
			XMUINT4 indices = 
			{ 
				s > 0 ? bvw[0].boneIndex : 0, 
				s > 1 ? bvw[1].boneIndex : 0, 
				s > 2 ? bvw[2].boneIndex : 0, 
				s > 3 ? bvw[3].boneIndex : 0 
			};

			vertices.emplace_back
			(
				XMFLOAT4{ sourceVertices[i].x, sourceVertices[i].y, sourceVertices[i].z, 1.0f},
				XMFLOAT2{ sourceUV[i].x, sourceUV[i].y },
				XMFLOAT3{ sourceNormals[i].x, sourceNormals[i].y, sourceNormals[i].z },
				indices,
				weights
			);
		}

		createVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void AnimationMaterial::createVertexBuffer(ID3D11Device* device, AnimationMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = getVertexSize() * vertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if(FAILED(hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
			throw new ApplicationException("CreateBuffer() error", hr);
	}

}
