#include "AnimationGame.h"

#include <format>

#include <dinput.h>

namespace Engine
{
	static HRESULT hr;
	const DirectX::XMVECTORF32 AnimationGame::s_BackgroundColor = { 0.09f, 0.64f, 0.9f, 1.0f };

	
	void AnimationGame::initialize()
	{
		if(FAILED(hr = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr);

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_TextPrinter = new TextPrinter(this, { 0.0f, 60.0f });
		m_Keyboard = new Keyboard(this, m_DirectInput);
		m_Mouse = new Mouse(this, m_DirectInput);
		m_FirstPersonCamera = new FirstPersonCamera(this);
		m_Skybox = new Skybox(this, m_FirstPersonCamera, L"resources\\skybox.dds", 500.0f);
		m_PointLightDemo = new PointLightDemo(this, m_FirstPersonCamera);
		m_AnimationDemo = new AnimationDemo(this, m_FirstPersonCamera);

		m_Components.push_back(m_Keyboard);
		m_Components.push_back(m_Mouse);
		m_Components.push_back(m_FirstPersonCamera);
		m_Components.push_back(m_Skybox);
		m_Components.push_back(m_PointLightDemo);
		m_Components.push_back(m_AnimationDemo);

		m_Components.push_back(m_FPSComponent);
		m_Components.push_back(m_TextPrinter);


		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);
		m_ServiceContainer.addService(Camera::typeIdClass(), m_FirstPersonCamera);

		Application::initialize();
	}

	void AnimationGame::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

		{
			std::stringstream text;
			XMFLOAT3 pos = m_FirstPersonCamera->getPosition();

			if(m_Keyboard->isKeyDown(DIK_W)) text << "W";
			text << "\nMouse X: " << m_Mouse->getX() << "  Y: " << m_Mouse->getY() << '\n';
		
			text << "Camera pos: " << std::format("{} {} {} ", pos.x, pos.y, pos.z) << "\nMat: \n";
			for(auto& vec : m_FirstPersonCamera->getViewMatrix().r)
			{
				for(auto& num : vec.m128_f32) text << std::format("{:8.3f} ", num);
				text << "\n";
			}
	
			XMFLOAT3 lightPosition = m_PointLightDemo->getLightPosition();

			text << "Point light position: " << std::format("{}, {}, {}", lightPosition.x, lightPosition.y, lightPosition.z);

			text << '\0';
			m_TextPrinter->setText(text.str());
		}



		Application::update(applicationTime);
	}

	void AnimationGame::draw(const ApplicationTime& applicationTime)
	{
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Application::draw(applicationTime);

		if(FAILED(hr = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr);
	}

	void AnimationGame::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}

}