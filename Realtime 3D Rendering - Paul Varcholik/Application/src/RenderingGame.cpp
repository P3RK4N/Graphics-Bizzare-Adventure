#include "RenderingGame.h"

#include <dinput.h>

namespace Engine
{


	HRESULT hr;
	const DirectX::XMVECTORF32 RenderingGame::s_BackgroundColor = { 0.8f, 0.2f, 0.9f, 1.0f };

	void RenderingGame::initialize()
	{
		if(FAILED(hr = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr);

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_TextPrinter = new TextPrinter(this, { 0.0f, 60.0f });
		m_Keyboard = new Keyboard(this, m_DirectInput);
		m_Mouse = new Mouse(this, m_DirectInput);

		m_Components.push_back(m_FPSComponent);
		m_Components.push_back(m_TextPrinter);
		m_Components.push_back(m_Keyboard);
		m_Components.push_back(m_Mouse);

		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);

		Application::initialize();
	}

	void RenderingGame::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

		std::stringstream text;
		
		RECT rect{};
		GetWindowRect(m_WindowHandle, &rect);
		text << "Window: " << rect.left << " " << rect.right << " " << rect.top << " " << rect.bottom << "\n";


		text << "Mouse X: " << m_Mouse->getX() << "  Y: " << m_Mouse->getY() << '\n';

		text << '\0';
		m_TextPrinter->setText(text.str());

		Application::update(applicationTime);
	}

	void RenderingGame::draw(const ApplicationTime& applicationTime)
	{
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Application::draw(applicationTime);

		if(FAILED(hr = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr);
	}

	void RenderingGame::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}

}