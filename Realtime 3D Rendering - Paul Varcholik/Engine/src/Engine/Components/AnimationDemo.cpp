#include "pch.h"
#include "AnimationDemo.h"

#include "Engine/Core/Application.h"
#include "Engine/Components/Camera.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/Mesh.h"

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(AnimationDemo)

	AnimationDemo::~AnimationDemo()
	{
		DeleteObject(m_Effect);
		DeleteObject(m_Material);
		DeleteObject(m_DirectionalLight);

		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
		ReleaseObject(m_TextureShaderResourceView);
	}

	void AnimationDemo::initialize()
	{
		m_Keyboard = (Keyboard*)m_Application->getServiceContainer().getService(Keyboard::typeIdClass());

		m_Model = new Model(m_Application, "resources\\run.dae", true);

		m_AnimationPlayer = new AnimationPlayer(m_Application, m_Model);
		m_AnimationPlayer->startClip(*(m_Model->getAnimationClipsByName().begin()->second));


		m_Effect = new Effect(m_Application);
		m_Effect->loadCompiledEffect(L"resources\\SkinnedModel.cso");
		m_Material = new AnimationMaterial();
		m_Material->initialize(m_Effect);

		Mesh* mesh = m_Model->getMeshes().at(0);
		m_Material->createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();

		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixTranslation(-5.0f, 2.0f, -5.0f) * XMMatrixIdentity());
		m_LightPosition = { -6.0f, 2.5f, -5.0f };
		m_LightRadius = 3.0f;

		m_SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_SpecularPower = 25.0f;
		m_AmbientColor = { 1.0f, 1.0f, 1.0f, 0.2f };
		m_DirectionalLight = new DirectionalLight({ 1.0f, 1.0f, 1.0f, 5.0f }, { 0.0f, 0.0f, 1.0f });

		if(FAILED(hr = CreateWICTextureFromFile
		(
			m_Application->getDirect3DDevice(),
			m_Application->getDirect3DDeviceContext(),
			L"resources\\run.png",
			nullptr,
			&m_TextureShaderResourceView
		))) throw new ApplicationException("CreateTexture() error", hr);

		DrawableApplicationComponent::initialize();
	}

	void AnimationDemo::update(const ApplicationTime& applicationTime)
	{
		float move = 10.0f * applicationTime.getElapsedApplicationTime();
		if(m_Keyboard->isKeyDown(DIK_DOWN)) m_LightPosition.y -= move;
		if(m_Keyboard->isKeyDown(DIK_UP)) m_LightPosition.y += move;

		m_AnimationPlayer->update(applicationTime);
	}

	void AnimationDemo::draw(const ApplicationTime& applicationTime)
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
		XMVECTOR specularColor = XMLoadColor(&m_SpecularColor);
		XMVECTOR lightPosition = XMLoadFloat3(&m_LightPosition);

		m_Material->getLightRadius() << m_LightRadius;
		m_Material->getSpecularColor() << specularColor;
		m_Material->getSpecularPower() << m_SpecularPower;
		m_Material->getLightPosition() << lightPosition;
		m_Material->getCameraPosition() << m_Camera->getPositionVector();
		m_Material->getWorldViewProjection() << wvp;
		m_Material->getWorld() << worldMatrix;
		m_Material->getAmbientColor() << ambientColor;
		m_Material->getLightColor() << m_DirectionalLight->getColorVector();
		m_Material->getColorTexture() << m_TextureShaderResourceView;
		m_Material->getBoneTransforms() << m_AnimationPlayer->getBoneTransforms();

		pass->apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void AnimationDemo::updateAmbientLight(const ApplicationTime& applicationTime)
	{
	}

	void AnimationDemo::updateDirectionalLight(const ApplicationTime& applicationTime)
	{
	}

}
