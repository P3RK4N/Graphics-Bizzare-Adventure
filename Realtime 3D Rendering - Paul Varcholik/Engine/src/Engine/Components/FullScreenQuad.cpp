#include "pch.h"
#include "FullScreenQuad.h"

#include "Engine/Core/Application.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(FullScreenQuad)

	FullScreenQuad::FullScreenQuad(Application* app)
		: DrawableApplicationComponent(app)
	{
	}

	FullScreenQuad::FullScreenQuad(Application* app, Material* material)
		: DrawableApplicationComponent(app), m_Material(material)
	{
	}

	FullScreenQuad::~FullScreenQuad()
	{
		ReleaseObject(m_IndexBuffer);
		ReleaseObject(m_VertexBuffer);
	}

	void FullScreenQuad::setMaterial(Material* material, const std::string& techniqueName, const std::string& passName)
	{
		m_Material = material;
		setActiveTechnique(techniqueName, passName);
	}

	void FullScreenQuad::setActiveTechnique(const std::string& techniqueName, const std::string& passName)
	{
		Technique* technique = m_Material->getEffect()->getTechniquesByName().at(techniqueName);
		assert(technique);

		m_Pass = technique->getPassesByName().at(passName);
		assert(m_Pass);
		m_InputLayout = m_Material->getInputLayouts().at(m_Pass);
	}

	void FullScreenQuad::setCustomUpdateMaterial(std::function<void()> callback)
	{
		m_CustomUpdateMaterial = callback;
	}

	void FullScreenQuad::initialize()
	{
		PositionTextureVertex vertices[] =
		{
			{ XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)	},
			{ XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)	},
			{ XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)	},
			{ XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)	}
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = vertices;

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer)))
			throw new ApplicationException();

		UINT indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		m_IndexCount = sizeof(indices) / sizeof(indices[0]);

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.ByteWidth = sizeof(indices);
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = indices;

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer)))
			throw new ApplicationException();
	}

	void FullScreenQuad::draw(const ApplicationTime& applicationTime)
	{
		assert(m_Pass);
		assert(m_InputLayout);

		ID3D11DeviceContext* deviceContext = m_Application->getDirect3DDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(m_InputLayout);

		UINT stride = sizeof(PositionTextureVertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		if(m_CustomUpdateMaterial) m_CustomUpdateMaterial();

		m_Pass->apply(0, deviceContext);
		deviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

}
