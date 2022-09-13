#include "pch.h"
#include "ProjectionMaterial.h"

#include "Engine/Core/Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(ProjectionMaterial)

	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, WorldViewProjection)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, WorldLightViewProjectionScale)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, WorldLightViewProjection)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, World)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, CameraPosition)

	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, AmbientColor)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, SpecularColor)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, SpecularPower)

	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, LightColor)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, LightPosition)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, LightDirection)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, LightRadius)

	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, ColorTexture)
	MATERIAL_VARIABLE_DEFINITION(ProjectionMaterial, ProjectedTexture)


	void ProjectionMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
		MATERIAL_VARIABLE_RETRIEVE(WorldLightViewProjectionScale)
		MATERIAL_VARIABLE_RETRIEVE(WorldLightViewProjection)
		MATERIAL_VARIABLE_RETRIEVE(World)
		MATERIAL_VARIABLE_RETRIEVE(CameraPosition)

		MATERIAL_VARIABLE_RETRIEVE(AmbientColor)
		MATERIAL_VARIABLE_RETRIEVE(SpecularColor)
		MATERIAL_VARIABLE_RETRIEVE(SpecularPower)

		MATERIAL_VARIABLE_RETRIEVE(LightColor)
		MATERIAL_VARIABLE_RETRIEVE(LightPosition)
		MATERIAL_VARIABLE_RETRIEVE(LightDirection)
		MATERIAL_VARIABLE_RETRIEVE(LightRadius)

		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)
		MATERIAL_VARIABLE_RETRIEVE(ProjectedTexture)

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("project_texture", "p0", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
		createInputLayout("project_texture", "shadow", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

	void ProjectionMaterial::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		const std::vector<XMFLOAT3>& sourceUV = *mesh.getTextureCoordinates().at(0);
		const std::vector<XMFLOAT3>& sourceNormals = mesh.getNormals();

		std::vector<ProjectionMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());

		for(UINT i = 0; i < sourceVertices.size(); i++)
			vertices.emplace_back
			(
				XMFLOAT4{ sourceVertices[i].x, sourceVertices[i].y, sourceVertices[i].z, 1.0f},
				XMFLOAT2{ sourceUV[i].x, sourceUV[i].y },
				XMFLOAT3{ sourceNormals[i].x, sourceNormals[i].y, sourceNormals[i].z }
			);
		

		createVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void ProjectionMaterial::createVertexBuffer(ID3D11Device* device, ProjectionMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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
