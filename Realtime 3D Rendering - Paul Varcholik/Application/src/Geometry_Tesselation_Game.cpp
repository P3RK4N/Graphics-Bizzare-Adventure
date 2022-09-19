#include "Geometry_Tesselation_Game.h"

#include <format>

#include <dinput.h>

namespace Engine
{
	static HRESULT hr;
	const DirectX::XMVECTORF32 Geometry_Tesselation_Game::s_BackgroundColor = { 0.09f, 0.64f, 0.9f, 1.0f };

	
	void Geometry_Tesselation_Game::initialize()
	{
		if(FAILED(hr = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr);

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_Keyboard = new Keyboard(this, m_DirectInput);
		m_Mouse = new Mouse(this, m_DirectInput);
		m_FirstPersonCamera = new FirstPersonCamera(this);
		m_Skybox = new Skybox(this, m_FirstPersonCamera, L"resources\\skybox.dds", 500.0f);
		m_TextPrinter = new TextPrinter(this, { 0.0f, 60.0f });

		m_GeometryMaterialDemo1 = new GeometryMaterialDemo(this, m_FirstPersonCamera, 0.0f, "displacement1.jpg");
		m_GeometryMaterialDemo2 = new GeometryMaterialDemo(this, m_FirstPersonCamera, 1.0f, "displacement2.jpg");
		m_GeometryMaterialDemo3 = new GeometryMaterialDemo(this, m_FirstPersonCamera, 2.0f, "displacement4.jpg");

		m_Components.push_back(m_Keyboard);
		m_Components.push_back(m_Mouse);
		m_Components.push_back(m_FirstPersonCamera);
		m_Components.push_back(m_Skybox);
		m_Components.push_back(m_FPSComponent);
		m_Components.push_back(m_TextPrinter);
		m_Components.push_back(m_GeometryMaterialDemo1);
		m_Components.push_back(m_GeometryMaterialDemo2);
		m_Components.push_back(m_GeometryMaterialDemo3);

		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);
		m_ServiceContainer.addService(Camera::typeIdClass(), m_FirstPersonCamera);

		Application::initialize();
	}

	void Geometry_Tesselation_Game::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

		{
			std::stringstream text;
			text << "Subdivision Factor: " << m_GeometryMaterialDemo1->m_SubdivisionFactor << '\n';
			text << "Subdivision Strength: " << m_GeometryMaterialDemo1->m_SubdivisionStrength << '\0';
			m_TextPrinter->setText(text.str());
		}

		Application::update(applicationTime);
	}

	void Geometry_Tesselation_Game::draw(const ApplicationTime& applicationTime)
	{
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Application::draw(applicationTime);

		if(FAILED(hr = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr);
	}

	void Geometry_Tesselation_Game::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}

}