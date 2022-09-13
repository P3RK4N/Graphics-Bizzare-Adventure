#include "pch.h"
#include "DiffuseMaterial.h"

#include "Engine/Core/Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(DiffuseLightingMaterial)

	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, WorldViewProjection)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, World)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, CameraPosition)

	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, AmbientColor)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, SpecularColor)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, SpecularPower)

	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, LightColor)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, LightPosition)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, LightDirection)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, LightRadius)

	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, ColorTexture)


	void DiffuseLightingMaterial::initialize(Effect* effect)
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
		MATERIAL_VARIABLE_RETRIEVE(LightDirection)
		MATERIAL_VARIABLE_RETRIEVE(LightRadius)

		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("main11", "p0", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

	void DiffuseLightingMaterial::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		const std::vector<XMFLOAT3>& sourceUV = *mesh.getTextureCoordinates().at(0);
		const std::vector<XMFLOAT3>& sourceNormals = mesh.getNormals();

		std::vector<DiffuseLightingMaterialVertex> vertices;
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

	void DiffuseLightingMaterial::createVertexBuffer(ID3D11Device* device, DiffuseLightingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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
