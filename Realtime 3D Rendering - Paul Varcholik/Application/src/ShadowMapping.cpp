#include "ShadowMapping.h"

#include <format>

#include <dinput.h>

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr2;
	const DirectX::XMVECTORF32 ShadowMapping::s_BackgroundColor = { 0.09f, 0.64f, 0.9f, 1.0f };

	
	void ShadowMapping::initialize()
	{
		if(FAILED(hr2 = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr2);

		m_ShadowMappingTarget = new DepthMap(this, 1024, 1024);

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_TextPrinter = new TextPrinter(this, { 0.0f, 60.0f });

		m_Keyboard = new Keyboard(this, m_DirectInput);
		m_Mouse = new Mouse(this, m_DirectInput);

		m_FirstPersonCamera = new FirstPersonCamera(this);
		m_LightCamera = new Camera(this);

		m_Skybox = new Skybox(this, m_FirstPersonCamera, L"resources\\skybox.dds", 500.0f);

		m_DiffuseLightingDemo = new DiffuseLightingDemo(this, m_FirstPersonCamera);
		m_PointLightDemo = new PointLightDemo(this, m_FirstPersonCamera);

		m_LightCamera = new Camera(this, XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 0.0f, 0.0f, 1.0f}, XMFLOAT3{ 0.0f, 1.0f, 0.0f });

		m_ProjectionDemo1 = new ProjectionDemo(this, m_FirstPersonCamera, m_ShadowMappingTarget, m_LightCamera->getViewProjectionMatrix(), XMFLOAT3{ 0.0f, 0.0f, 1.6f});
		m_ProjectionDemo2 = new ProjectionDemo(this, m_FirstPersonCamera, m_ShadowMappingTarget, m_LightCamera->getViewProjectionMatrix(), XMFLOAT3{ 1.0f, 1.2f, 4.3f}, false);

		m_Components.push_back(m_Keyboard);
		m_Components.push_back(m_Mouse);
		m_Components.push_back(m_FirstPersonCamera);
		m_Components.push_back(m_Skybox);
		m_Components.push_back(m_DiffuseLightingDemo);
		m_Components.push_back(m_PointLightDemo);
		m_Components.push_back(m_ProjectionDemo1);
		m_Components.push_back(m_ProjectionDemo2);

		m_Components.push_back(m_FPSComponent);
		m_Components.push_back(m_TextPrinter);

		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);
		m_ServiceContainer.addService(Camera::typeIdClass(), m_FirstPersonCamera);

		Application::initialize();
	}

	void ShadowMapping::updateText()
	{
		std::stringstream text;
		XMFLOAT3 pos = m_FirstPersonCamera->getPosition();

		if(m_Keyboard->isKeyDown(DIK_W)) text << "W";
		text << "\nMouse X: " << m_Mouse->getX() << "  Y: " << m_Mouse->getY() << '\n';
		
		text << "Camera pos: " << std::format("{} {} {} ", pos.x, pos.y, pos.z) << "\nMat: \n";
		for(auto& vec : m_FirstPersonCamera->getViewProjectionMatrix().r)
		{
			for(auto& num : vec.m128_f32) text << std::format("{:8.3f} ", num);
			text << "\n";
		}
		
		XMFLOAT4 col;
		XMStoreFloat4(&col, m_DiffuseLightingDemo->getAmbientColor());
		XMFLOAT3 lightPosition = m_PointLightDemo->getLightPosition();

		text << col.w << '\n';
		text << "Point light position: " << std::format("{}, {}, {}", lightPosition.x, lightPosition.y, lightPosition.z);

		text << '\0';
		m_TextPrinter->setText(text.str());
	}

	void ShadowMapping::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

		updateText();

		Application::update(applicationTime);
	}

	void ShadowMapping::draw(const ApplicationTime& applicationTime)
	{
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));

		m_ShadowMappingTarget->begin();
		m_IsShadowPass = true;

		Application::draw(applicationTime);

		m_ShadowMappingTarget->end();
		m_IsShadowPass = false;


		Application::draw(applicationTime);

		if(FAILED(hr2 = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr2);
	}

	void ShadowMapping::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}
}