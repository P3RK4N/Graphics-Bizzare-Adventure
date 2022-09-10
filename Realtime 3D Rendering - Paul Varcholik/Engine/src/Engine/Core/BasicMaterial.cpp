#include "pch.h"
#include "BasicMaterial.h"

#include "Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(BasicMaterial)

	MATERIAL_VARIABLE_DEFINITION(BasicMaterial, WorldViewProjection)

	BasicMaterial::BasicMaterial() :
		Material("main11") {}

	void BasicMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection);

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("main11", "p0", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

	void BasicMaterial::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		std::vector<BasicMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());

		if(mesh.getVertexColors().size() > 0)
		{
			std::vector<XMFLOAT4>* vertexColors = mesh.getVertexColors().at(0);

			assert(vertexColors->size() == sourceVertices.size());

			for(UINT i = 0; i < sourceVertices.size(); i++)
			{
				XMFLOAT3 position = sourceVertices.at(i);
				XMFLOAT4 color = vertexColors->at(i);
				vertices.push_back(BasicMaterialVertex{ XMFLOAT4{position.x, position.y, position.z, 1.0f} , color });
			}
		}
		else
		{
			for(UINT i = 0; i < sourceVertices.size(); i++)
			{
				XMFLOAT3 position = sourceVertices.at(i);
				vertices.push_back(BasicMaterialVertex
					{ 
						XMFLOAT4{position.x, position.y, position.z, 1.0f}, 
						{ (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, 1.0f }
					});
			}
		}

		createVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void BasicMaterial::createVertexBuffer(ID3D11Device* device, BasicMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

	UINT BasicMaterial::getVertexSize() const
	{
		return sizeof(BasicMaterialVertex);
	}
}
