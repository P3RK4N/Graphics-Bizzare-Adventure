#include "pch.h"
#include "MaterialDemo.h"

#include "Engine/Shader/Effect.h"
#include "Engine/Materials/BasicMaterial.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Mesh.h"
#include "Engine/Components/Camera.h"

namespace Engine
{
	RTTI_DEFINITIONS(MaterialDemo)

	MaterialDemo::MaterialDemo(Application* app, Camera* camera)
		: DrawableApplicationComponent(app, camera)
	{
		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(5.0f, 0.0f, 0.0f) * XMMatrixIdentity());
	}	

	MaterialDemo::~MaterialDemo()
	{
		DeleteObject(m_BillboardingMaterial);
		DeleteObject(m_GeometryEffect);
		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
	}

	void MaterialDemo::initialize()
	{
		Scope<Model> model = createScope<Model>(m_Application, "resources\\blenderSphere.obj", true);

		m_GeometryEffect = new Effect(m_Application);
		m_GeometryEffect->loadCompiledEffect(L"resources\\basic.cso");
		m_BillboardingMaterial = new BasicMaterial();
		m_BillboardingMaterial->initialize(m_GeometryEffect);

		Mesh* mesh = model->getMeshes().at(0);
		m_BillboardingMaterial->createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();
	}

	void MaterialDemo::draw(const ApplicationTime& applicationTime)
	{
		//Set primitive topology for input assembler stage
		ID3D11DeviceContext1* deviceContext = m_Application->getDirect3DDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//Bind input layout to input assembler stage
		Technique* technique = m_BillboardingMaterial->getCurrentTechnique();
		Pass* pass = technique->getPasses().at(0);
		deviceContext->IASetInputLayout(m_BillboardingMaterial->getInputLayouts().at(pass));
		//Bind vertex buffer to input assembler stage
		UINT stride = m_BillboardingMaterial->getVertexSize();
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		//Bind index buffer to input assembler stage
		deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//Set shader constants
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();
		//TMP
		worldMatrix = XMMatrixRotationY(1.0f * (float)applicationTime.getElapsedApplicationTime()) * worldMatrix;
		XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);
		//----
		m_BillboardingMaterial->getWorldViewProjection() << wvp;
		//Apply effect pass
		pass->apply(0, deviceContext);
		//Execute indexed draw
		deviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

}
