#include "pch.h"
#include "DiffuseLightingDemo.h"

#include "Engine/Core/Application.h"
#include "Engine/Components/Camera.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/Mesh.h"

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(DiffuseLightingDemo)

	DiffuseLightingDemo::~DiffuseLightingDemo()
	{
		DeleteObject(m_Effect);
		DeleteObject(m_Material);
		DeleteObject(m_DirectionalLight);

		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
		ReleaseObject(m_TextureShaderResourceView);
	}

	void DiffuseLightingDemo::initialize()
	{
		m_Keyboard = (Keyboard*)m_Application->getServiceContainer().getService(Keyboard::typeIdClass());

		Scope<Model> model = createScope<Model>(m_Application, "resources\\blenderSphere.obj", true);

		m_Effect = new Effect(m_Application);
		m_Effect->loadCompiledEffect(L"resources\\diffuse.cso");
		m_Material = new DiffuseLightingMaterial();
		m_Material->initialize(m_Effect);

		Mesh* mesh = model->getMeshes().at(0);
		m_Material->createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();

		m_AmbientColor = { 1.0f, 1.0f, 1.0f, 0.2f };
		m_DirectionalLight = new DirectionalLight({ 1.0f, 1.0f, 1.0f, 5.0f }, { 0.0f, 0.0f, 1.0f });

		if(FAILED(hr = CreateWICTextureFromFile
		(
			m_Application->getDirect3DDevice(),
			m_Application->getDirect3DDeviceContext(),
			L"resources\\earth2.jpg",
			nullptr,
			&m_TextureShaderResourceView
		))) throw new ApplicationException("CreateTexture() error", hr);

		DrawableApplicationComponent::initialize();
	}

	void DiffuseLightingDemo::update(const ApplicationTime& applicationTime)
	{
		XMStoreFloat4x4
		(
			&m_WorldMatrix,
			XMMatrixRotationY(-0.5f * (float)applicationTime.getElapsedApplicationTime()) *
			XMLoadFloat4x4(&m_WorldMatrix)
		);

		if(m_Keyboard->isKeyDown(DIK_DOWN)) m_AmbientColor.a = max(0, m_AmbientColor.a - 1);
		if(m_Keyboard->isKeyDown(DIK_UP)) m_AmbientColor.a = min(254, m_AmbientColor.a + 1);
	}

	void DiffuseLightingDemo::draw(const ApplicationTime& applicationTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = m_Application->getDirect3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		Pass* pass = m_Material->getCurrentTechnique()->getPasses().at(0);
		ID3D11InputLayout* inputLayout = m_Material->getInputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);
		
		UINT stride = m_Material->getVertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		
		direct3DDeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();
		XMVECTOR ambientColor = XMLoadColor(&m_AmbientColor);
		m_Material->getWorldViewProjection() << wvp;
		m_Material->getWorld() << worldMatrix;
		m_Material->getAmbientColor() << ambientColor;
		m_Material->getLightColor() << m_DirectionalLight->getColorVector();
		m_Material->getLightDirection() << m_DirectionalLight->getDirectionVector();
		m_Material->getColorTexture() << m_TextureShaderResourceView;

		pass->apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void DiffuseLightingDemo::updateAmbientLight(const ApplicationTime& applicationTime)
	{
	}

	void DiffuseLightingDemo::updateDirectionalLight(const ApplicationTime& applicationTime)
	{
	}

}
