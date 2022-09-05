#include "RenderingGame.h"
#include <Engine.h>

namespace Engine
{
	HRESULT hr;
	const DirectX::XMVECTORF32 RenderingGame::s_BackgroundColor = { 0.8f, 0.2f, 0.9f, 1.0f };

	void RenderingGame::initialize()
	{
		m_Components.push_back(new FPSComponent(*reinterpret_cast<Application*>(this)));

		Application::initialize();
	}

	void RenderingGame::update(const ApplicationTime& applicationTime)
	{

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

		Application::shutdown();
	}

}