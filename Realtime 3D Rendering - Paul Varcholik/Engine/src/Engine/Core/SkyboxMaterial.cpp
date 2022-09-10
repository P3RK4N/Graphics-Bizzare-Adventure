#include "pch.h"
#include "SkyboxMaterial.h"

#include "Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(SkyboxMaterial)

	MATERIAL_VARIABLE_DEFINITION(SkyboxMaterial, WorldViewProjection)
	MATERIAL_VARIABLE_DEFINITION(SkyboxMaterial, SkyboxTexture)

	SkyboxMaterial::SkyboxMaterial()
		: Material("main11") {}

	void SkyboxMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection);
		MATERIAL_VARIABLE_RETRIEVE(SkyboxTexture);


		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("main11", "p0", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

	void SkyboxMaterial::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		std::vector<XMFLOAT4> vertices;
		vertices.reserve(sourceVertices.size());
		for(UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			vertices.emplace_back(position.x, position.y, position.z, 1.0f);
		}
		createVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void SkyboxMaterial::createVertexBuffer(ID3D11Device* device, XMFLOAT4* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

	UINT SkyboxMaterial::getVertexSize() const
	{
		return sizeof(XMFLOAT4);
	}
}

