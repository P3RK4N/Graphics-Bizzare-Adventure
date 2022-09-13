#include "pch.h"
#include "ProjectionDemo.h"

#include "Engine/Core/Application.h"
#include "Engine/Components/Camera.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/Mesh.h"

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr;

	static XMFLOAT4X4 s_TextureScalingMatrix =
	{
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1
	};

	RTTI_DEFINITIONS(ProjectionDemo)

	ProjectionDemo::~ProjectionDemo()
	{
		DeleteObject(m_Effect);
		DeleteObject(m_Material);
		DeleteObject(m_DirectionalLight);

		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
		ReleaseObject(m_TextureShaderResourceView);
	}

	void ProjectionDemo::initialize()
	{
		m_Keyboard = (Keyboard*)m_Application->getServiceContainer().getService(Keyboard::typeIdClass());

		Scope<Model> model = createScope<Model>(m_Application, "resources\\blenderSphere.obj", true);

		m_Effect = new Effect(m_Application);
		m_Effect->loadCompiledEffect(L"resources\\PTM.cso");
		m_Material = new ProjectionMaterial();
		m_Material->initialize(m_Effect);

		Mesh* mesh = model->getMeshes().at(0);
		m_Material->createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();

		m_LightPosition = { 0.0f, 0.0f, 0.0f };
		m_LightRadius = 8.0f;

		m_SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_SpecularPower = 25.0f;
		m_AmbientColor = { 1.0f, 1.0f, 1.0f, 0.4f };
		m_DirectionalLight = new DirectionalLight({ 1.0f, 1.0f, 1.0f, 10.0f }, { 0.0f, 0.0f, 1.0f });

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

	void ProjectionDemo::update(const ApplicationTime& applicationTime)
	{
		if(!m_First)
			XMStoreFloat4x4
			(
				&m_WorldMatrix,
				XMMatrixRotationY(-0.5f * (float)applicationTime.getElapsedApplicationTime()) *
				XMLoadFloat4x4(&m_WorldMatrix)
			);
		else
			XMStoreFloat4x4
			(
				&m_WorldMatrix,
				XMMatrixRotationY(-0.5f * (float)applicationTime.getElapsedApplicationTime()) *
				XMMatrixTranslation(/*sin(applicationTime.getTotalApplicationTime())*/+1.0f, 0.0f, 1.6f)
			);



		float move = 10.0f * applicationTime.getElapsedApplicationTime();
		if(m_Keyboard->isKeyDown(DIK_DOWN)) m_LightPosition.y -= move;
		if(m_Keyboard->isKeyDown(DIK_UP)) m_LightPosition.y += move;
	}

	void ProjectionDemo::draw(const ApplicationTime& applicationTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = m_Application->getDirect3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		Pass* pass = m_Application->isShadowPass() ? m_Material->getCurrentTechnique()->getPassesByName().at("shadow") : m_Material->getCurrentTechnique()->getPassesByName().at("p0");
		ID3D11InputLayout* inputLayout = m_Material->getInputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);
		
		UINT stride = m_Material->getVertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		
		direct3DDeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//----------------------------
		XMVECTOR ambientColor = XMLoadColor(&m_AmbientColor);
		XMVECTOR specularColor = XMLoadColor(&m_SpecularColor);
		XMVECTOR lightPosition = XMLoadFloat3(&m_LightPosition);

		m_Material->getLightRadius() << m_LightRadius;
		m_Material->getAmbientColor() << ambientColor;
		m_Material->getSpecularColor() << specularColor;
		m_Material->getSpecularPower() << m_SpecularPower;
		m_Material->getLightPosition() << lightPosition;
		m_Material->getLightColor() << m_DirectionalLight->getColorVector();
		m_Material->getLightDirection() << m_DirectionalLight->getDirectionVector();
		m_Material->getCameraPosition() << m_Camera->getPositionVector();
		//-------------------------------------

		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX lvp = XMLoadFloat4x4(&m_LightViewProj);

		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();
		m_Material->getWorld() << worldMatrix;
		m_Material->getWorldViewProjection() << wvp;

		XMMATRIX textureScalingMatrix = XMLoadFloat4x4(&s_TextureScalingMatrix);
		XMMATRIX wlvp = worldMatrix * lvp;
		XMMATRIX wlvps = wlvp * textureScalingMatrix;
		m_Material->getWorldLightViewProjection() << wlvp;
		m_Material->getWorldLightViewProjectionScale() << wlvps;





		m_Material->getColorTexture() << m_TextureShaderResourceView;
		m_Material->getProjectedTexture() << m_DepthMap->getOutputDepth();

		pass->apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void ProjectionDemo::updateAmbientLight(const ApplicationTime& applicationTime)
	{
	}

	void ProjectionDemo::updateDirectionalLight(const ApplicationTime& applicationTime)
	{
	}

}
