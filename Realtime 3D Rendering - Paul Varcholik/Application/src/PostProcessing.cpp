#include "PostProcessing.h"

#include <format>

#include <dinput.h>

#include <WICTextureLoader.h>

namespace Engine
{
	static HRESULT hr2;
	const DirectX::XMVECTORF32 PostProcessing::s_BackgroundColor = { 0.09f, 0.64f, 0.9f, 1.0f };

	
	void PostProcessing::initialize()
	{
		if(FAILED(hr2 = DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
			throw new ApplicationException("DirectInput8Create() error", hr2);

		if(FAILED(hr2 = CreateWICTextureFromFile(m_Direct3DDevice, L"resources\\distortion.png", &m_DistortionTexture, &m_DistortionView)))
			throw new ApplicationException("", hr2);

		m_DepthMap = new FullScreenRenderTarget(this);
		m_PostProcessingEffect = new Effect(this);
		m_PostProcessingEffect->loadCompiledEffect(L"resources\\PostProcessingEffect.cso");
		m_PostProcessMaterial = new PostProcessMaterial();
		m_PostProcessMaterial->initialize(m_PostProcessingEffect);
		m_FullScreenQuad = new FullScreenQuad(this, m_PostProcessMaterial);
		m_FullScreenQuad->initialize();
		m_FullScreenQuad->setActiveTechnique("filter", "distortion");
		m_FullScreenQuad->setCustomUpdateMaterial(std::bind(&PostProcessing::updatePostProcessMaterial, this));

		m_FPSComponent = new FPSComponent(this, { 0.0f, 0.0f });
		m_TextPrinter = new TextPrinter(this, { 0.0f, 60.0f });
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
		m_Components.push_back(m_TextPrinter);


		m_ServiceContainer.addService(Keyboard::typeIdClass(), m_Keyboard);
		m_ServiceContainer.addService(Mouse::typeIdClass(), m_Mouse);
		m_ServiceContainer.addService(Camera::typeIdClass(), m_FirstPersonCamera);

		Application::initialize();
	}

	void PostProcessing::update(const ApplicationTime& applicationTime)
	{
		if(m_Keyboard->wasKeyPressedThisFrame(DIK_ESCAPE))
			exit();

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
		
		XMFLOAT4 col;
		XMStoreFloat4(&col, m_DiffuseLightingDemo->getAmbientColor());
		XMFLOAT3 lightPosition = m_PointLightDemo->getLightPosition();

		text << col.w << '\n';
		text << "Point light position: " << std::format("{}, {}, {}", lightPosition.x, lightPosition.y, lightPosition.z);

		text << '\0';
		m_TextPrinter->setText(text.str());

		Application::update(applicationTime);
	}

	void PostProcessing::draw(const ApplicationTime& applicationTime)
	{
		m_DepthMap->begin();

		m_Direct3DDeviceContext->ClearRenderTargetView(m_DepthMap->getRenderTargetView() , reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthMap->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Application::draw(applicationTime);

		m_DepthMap->end();

		
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&s_BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_FullScreenQuad->draw(applicationTime);

		if(FAILED(hr2 = m_SwapChain->Present(m_VSyncEnabled, 0)))
			throw ApplicationException("SwapChain exception!", hr2);

		static ID3D11ShaderResourceView* null[]{ nullptr, nullptr };
		m_Direct3DDeviceContext->PSSetShaderResources(0, 2, null);
	}

	void PostProcessing::shutdown()
	{
		for(auto& component : m_Components)
			DeleteObject(component);

		ReleaseObject(m_DirectInput);

		Application::shutdown();
	}



	void PostProcessing::updatePostProcessMaterial()
	{
		m_PostProcessMaterial->getColorTexture() << m_DepthMap->getOutputColor();
		m_PostProcessMaterial->getDistortionTexture() << m_DistortionView;
	}

}