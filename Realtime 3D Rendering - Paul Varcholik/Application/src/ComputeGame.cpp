#include "ComputeGame.h"

#include <format>

#include <dinput.h>

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr2;
	const DirectX::XMVECTORF32 ComputeGame::s_BackgroundColor = { 0.09f, 0.64f, 0.9f, 1.0f };

	
	void ComputeGame::initialize()
	{
		if(FAILED(hr2 = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr2);

		if(FAILED(hr2 = CreateWICTextureFromFile(m_Direct3DDevice, L"resources\\distortion.png", &m_DistortionTexture, &m_DistortionView)))
			throw new ApplicationException("", hr2);

		m_ComputeTarget = new ComputeTarget(this);
		
		m_PostProcessEffect = new Effect(this);
		m_PostProcessEffect->loadCompiledEffect(L"resources\\PostProcessingEffect.cso");
		m_PostProcessMaterial = new PostProcessMaterial();
		m_PostProcessMaterial->initialize(m_PostProcessEffect);

		m_FullScreenQuad = new FullScreenQuad(this, m_PostProcessMaterial);
		m_FullScreenQuad->initialize();
		m_FullScreenQuad->setActiveTechnique("filter", "nothing");
		m_FullScreenQuad->setCustomUpdateMaterial(std::bind(&ComputeGame::updatePostProcessMaterial, this));

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_Keyboard = new Keyboard(this, m_DirectInput);
		m_Mouse = new Mouse(this, m_DirectInput);
		m_FirstPersonCamera = new FirstPersonCamera(this);
		m_Skybox = new Skybox(this, m_FirstPersonCamera, L"resources\\skybox.dds", 500.0f);
		m_DiffuseLightingDemo = new DiffuseLightingDemo(this, m_FirstPersonCamera);
		m_PointLightDemo = new PointLightDemo(this, m_FirstPersonCamera);

		m_Components.push_back(m_Keyboard);
		m_Components.push_back(m_Mouse);
		m_Components.push_back(m_FirstPersonCamera);
		m_Components.push_back(m_Skybox);
		m_Components.push_back(m_DiffuseLightingDemo);
		m_Components.push_back(m_PointLightDemo);

		m_Components.push_back(m_FPSComponent);


		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);
		m_ServiceContainer.addService(Camera::typeIdClass(), m_FirstPersonCamera);

		Application::initialize();
	}

	void ComputeGame::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

		Application::update(applicationTime);
	}

	void ComputeGame::draw(const ApplicationTime& applicationTime)
	{
		m_Direct3DDeviceContext->ClearUnorderedAccessViewFloat(m_ComputeTarget->getUAV() , reinterpret_cast<const float*>(&s_BackgroundColor));
		m_ComputeTarget->draw(applicationTime);

		
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_FullScreenQuad->draw(applicationTime);

		if(FAILED(hr2 = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr2);

		static ID3D11ShaderResourceView* null[]{ nullptr };
		m_Direct3DDeviceContext->PSSetShaderResources(0, 1, null);
	}

	void ComputeGame::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}



	void ComputeGame::updatePostProcessMaterial()
	{
		m_PostProcessMaterial->getColorTexture() << m_ComputeTarget->getOutputColor();
		m_PostProcessMaterial->getDistortionTexture() << m_DistortionView;
	}

}