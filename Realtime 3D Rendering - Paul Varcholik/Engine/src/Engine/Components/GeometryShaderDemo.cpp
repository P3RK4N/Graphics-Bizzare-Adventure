#include "pch.h"
#include "GeometryShaderDemo.h"

#include "Engine/Shader/Effect.h"
#include "Engine/Materials/BillboardingMaterial.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Mesh.h"
#include "Engine/Components/Camera.h"
#include "Engine/Core/RenderStateHelper.h"

#include <WICTextureLoader.h>

#define VERTEX_COUNT 5
#define TRIANGLE_COUNT 3
#define QUAD_COUNT 1 * 4

namespace Engine
{
	static HRESULT hr;

	namespace Utility
	{
		static std::wstring s2ws(const std::string& str)
		{
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo( size_needed, 0 );
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}
	}

	RTTI_DEFINITIONS(GeometryMaterialDemo)

	GeometryMaterialDemo::GeometryMaterialDemo(Application* app, Camera* camera, float position, std::string image)
		: DrawableApplicationComponent(app, camera), m_Position(position), m_Image(image)
	{
	}	

	GeometryMaterialDemo::~GeometryMaterialDemo()
	{
		DeleteObject(m_BillboardingMaterial);
		DeleteObject(m_GeometryEffect);
		ReleaseObject(m_VertexBuffer);
	}

	void GeometryMaterialDemo::initialize()
	{
		m_Keyboard = (Keyboard*)m_Application->getServiceContainer().getService(Keyboard::typeIdClass());
		XMStoreFloat4x4(&m_TextureMatrix, XMMatrixIdentity());
		m_GeometryEffect = new Effect(m_Application);
		m_GeometryEffect->loadCompiledEffect(L"resources\\SphericalBillboarding.cso");
		m_BillboardingMaterial = new BillboardingMaterial();
		m_BillboardingMaterial->initialize(m_GeometryEffect);

		BillboardingMaterialVertex vertices[QUAD_COUNT] = 
			{ 
				{ XMFLOAT4{ m_Position + 0.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 1.0f, 1.0f } }, 
				{ XMFLOAT4{ m_Position + 1.0f, 0.0f, 0.0f, 1.0f }, XMFLOAT2{ 0.0f, 1.0f } },
				{ XMFLOAT4{ m_Position + 0.0f, 0.0f, 1.0f, 1.0f }, XMFLOAT2{ 1.0f, 0.0f } },
				{ XMFLOAT4{ m_Position + 1.0f, 0.0f, 1.0f, 1.0f }, XMFLOAT2{ 0.0f, 0.0f } }
			};

		m_BillboardingMaterial->createVertexBuffer(m_Application->getDirect3DDevice(), vertices, QUAD_COUNT, &m_VertexBuffer);

		if(FAILED(hr = CreateWICTextureFromFile
		(
			m_Application->getDirect3DDevice(),
			m_Application->getDirect3DDeviceContext(),
			Utility::s2ws("resources\\" + m_Image).c_str(),
			nullptr,
			&m_ColorTextureView
		))) throw new ApplicationException("CreateTexture() error", hr);
	}

	void GeometryMaterialDemo::draw(const ApplicationTime& applicationTime)
	{
		ID3D11DeviceContext1* deviceContext = m_Application->getDirect3DDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		//Bind input layout to input assembler stage
		Technique* technique = m_BillboardingMaterial->getCurrentTechnique();
		Pass* pass = technique->getPasses().at(0);
		deviceContext->IASetInputLayout(m_BillboardingMaterial->getInputLayouts().at(pass));
		//Bind vertex buffer to input assembler stage
		UINT stride = m_BillboardingMaterial->getVertexSize();
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		//----
		XMMATRIX texMat = XMLoadFloat4x4(&m_TextureMatrix) * XMMatrixTranslation(0.0003f, 0.0f, 0.0007f);
		XMStoreFloat4x4(&m_TextureMatrix, texMat);

		m_BillboardingMaterial->getViewProjection() << m_Camera->getViewProjectionMatrix();
		m_BillboardingMaterial->getCameraPosition() << m_Camera->getPositionVector();
		m_BillboardingMaterial->getCameraUp() << m_Camera->getUpVector();
		m_BillboardingMaterial->getColorTexture() << m_ColorTextureView;
		m_BillboardingMaterial->getSubdivisionFactor() << m_SubdivisionFactor;
		m_BillboardingMaterial->getSubdivisionStrength() << m_SubdivisionStrength;
		m_BillboardingMaterial->getTextureMatrix() << texMat;
		//Apply effect pass
		pass->apply(0, deviceContext);
		deviceContext->Draw(QUAD_COUNT, 0);
	}

	void GeometryMaterialDemo::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->isKeyDown(DIK_UP)) m_SubdivisionFactor = min(200.0f, m_SubdivisionFactor + 0.25f);
		if(m_Keyboard->isKeyDown(DIK_DOWN)) m_SubdivisionFactor = max(1.0f, m_SubdivisionFactor - 0.25f);

		if(m_Keyboard->isKeyDown(DIK_RIGHT)) m_SubdivisionStrength = min(10.0f, m_SubdivisionStrength + 0.002f);
		if(m_Keyboard->isKeyDown(DIK_LEFT)) m_SubdivisionStrength = max(0.0f, m_SubdivisionStrength - 0.002f);
	}

}
