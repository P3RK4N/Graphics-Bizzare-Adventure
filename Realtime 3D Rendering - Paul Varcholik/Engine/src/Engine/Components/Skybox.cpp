#include "pch.h"
#include "Skybox.h"

#include "Engine/Core/Model.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Mesh.h"
#include "Engine/Components/Camera.h"

#include <DDSTextureLoader.h>

#define ROTATION_SPEED 0.1f

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(Skybox)

	Skybox::Skybox(Application* app, Camera* camera, const std::wstring& cubeMapFileName, float scale)
		: DrawableApplicationComponent(app, camera), m_CubeMapFileName(cubeMapFileName)
	{
		XMStoreFloat4x4(&m_ScaleMatrix, XMMatrixScaling(scale, scale, scale));
	}

	Skybox::~Skybox()
	{
		DeleteObject(m_Material);
		DeleteObject(m_Effect);
		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
	}

	void Skybox::initialize()
	{
		Scope<Model> model = createScope<Model>(m_Application, "resources\\blenderSphere.obj", true);

		m_Effect = new Effect(m_Application);
		m_Effect->loadCompiledEffect(L"resources\\skybox.cso");
		m_Material = new SkyboxMaterial();
		m_Material->initialize(m_Effect);

		Mesh* mesh = model->getMeshes().at(0);
		m_Material->createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();

		if(FAILED(hr = CreateDDSTextureFromFile
		(
			m_Application->getDirect3DDevice(),
			m_CubeMapFileName.c_str(),
			nullptr,
			&m_CubeMapShaderResourceView
		))) throw new ApplicationException("CreateDDSTextureFromFile() error", hr);
	}

	void Skybox::update(const ApplicationTime& applicationTime)
	{
		const XMFLOAT3& position = m_Camera->getPosition();
		XMStoreFloat4x4
		(
			&m_WorldMatrix,
			XMLoadFloat4x4(&m_ScaleMatrix) *
			XMMatrixRotationY(ROTATION_SPEED * (float)applicationTime.getTotalApplicationTime()) *
			XMMatrixTranslation(position.x, position.y,position.z)
		);
	}

	void Skybox::draw(const ApplicationTime& applicationTime)
	{
		//Set primitive topology for input assembler stage
		ID3D11DeviceContext1* deviceContext = m_Application->getDirect3DDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//Bind input layout to input assembler stage
		Technique* technique = m_Material->getCurrentTechnique();
		Pass* pass = technique->getPasses().at(0);
		deviceContext->IASetInputLayout(m_Material->getInputLayouts().at(pass));
		//Bind vertex buffer to input assembler stage
		UINT stride = m_Material->getVertexSize();
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		//Bind index buffer to input assembler stage
		deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//Set shader constants
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();
		m_Material->getWorldViewProjection() << wvp;
		m_Material->getSkyboxTexture() << m_CubeMapShaderResourceView;
		//Apply effect pass
		pass->apply(0, deviceContext);
		//Execute indexed draw
		deviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}
}