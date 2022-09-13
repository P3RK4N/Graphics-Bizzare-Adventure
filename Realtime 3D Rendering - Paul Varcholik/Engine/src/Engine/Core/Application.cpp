#include "pch.h"
#include "Application.h"

namespace Engine
{
	static HRESULT hr;

	void Application::initializeWindow()
	{
		ZeroMemory(&m_Window, sizeof(m_Window));
		m_Window.cbSize = sizeof(WNDCLASSEX);
		m_Window.style = CS_CLASSDC;
		m_Window.lpfnWndProc = WndProc;
		m_Window.hInstance = m_Instance;
		m_Window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		m_Window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		m_Window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		m_Window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		m_Window.lpszClassName = m_WindowClass.c_str();

		RECT windowRectangle = { 0, 0, m_ScreenWidth, m_ScreenHeight };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&m_Window);

		POINT center = centerWindow(m_ScreenWidth, m_ScreenHeight);
		m_WindowHandle = CreateWindow(m_WindowClass.c_str(), m_WindowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, m_Instance, nullptr);
		ShowWindow(m_WindowHandle, m_ShowCommand);
		UpdateWindow(m_WindowHandle);
	}

	void Application::initializeDirectX()
	{
		//Device------------------------------------------------
		UINT flags = 0;
		const D3D_FEATURE_LEVEL featureLevels[]{D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0};
		ID3D11Device* d3DDevice;
		ID3D11DeviceContext* d3DDeviceContext;

#ifdef DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		if(FAILED(hr = D3D11CreateDevice
		(
			NULL, 
			D3D_DRIVER_TYPE_HARDWARE, 
			NULL, 
			flags, 
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, 
			&d3DDevice,
			&m_FeatureLevel,
			&d3DDeviceContext
		))) throw ApplicationException("D3D11CreateDevice() failed", hr);

		if(FAILED(hr = d3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_Direct3DDevice))))
			throw ApplicationException("QueryInterface() failed", hr);
		if(FAILED(hr = d3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_Direct3DDeviceContext))))
			throw ApplicationException("QueryInterface() failed", hr);

		ReleaseObject(d3DDevice);
		ReleaseObject(d3DDeviceContext);

		//MulstiSampling------------------------------------------------
		m_Direct3DDevice->CheckMultisampleQualityLevels
		(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			m_MultiSamplingCount,
			&m_MultiSamplingQualityLevels
		);

		if(!m_MultiSamplingQualityLevels) throw ApplicationException("CheckMultisampleQualityLevels() failed", m_MultiSamplingQualityLevels);

		//SwapChain -------------------------------------------------
		DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDescription;
		ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));
		ZeroMemory(&swapChainFullScreenDescription, sizeof(swapChainFullScreenDescription));

		swapChainDescription.Width = m_ScreenWidth;
		swapChainDescription.Height = m_ScreenHeight;
		swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.SampleDesc.Count = m_IsMultiSamplingEnabled ? m_MultiSamplingCount : 1;
		swapChainDescription.SampleDesc.Quality = m_IsMultiSamplingEnabled ? m_MultiSamplingQualityLevels - 1 : 0;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainFullScreenDescription.RefreshRate = { 144, 1 };
		swapChainFullScreenDescription.Windowed = !m_IsFullScreen;

		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory2* dxgiFactory = nullptr;

		if(FAILED(hr = m_Direct3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
			throw ApplicationException("QueryInterface() failed", hr);

		if(FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
		{
			ReleaseObject(dxgiDevice);
			throw ApplicationException("GetParent() failed", hr);
		}

		if(FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			throw ApplicationException("GetParent() failed", hr);
		}

		if(FAILED(hr = dxgiFactory->CreateSwapChainForHwnd
		(
			dxgiDevice,
			m_WindowHandle,
			&swapChainDescription,
			&swapChainFullScreenDescription,
			nullptr,
			&m_SwapChain
		)))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			ReleaseObject(dxgiFactory);
		
			throw ApplicationException("QueryInterface() failed", hr);
		}

		ReleaseObject(dxgiDevice);
		ReleaseObject(dxgiAdapter);
		ReleaseObject(dxgiFactory);

		//Render Target--------------------------------------------------
		ID3D11Texture2D* backBuffer;
		if(FAILED(hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
			throw ApplicationException("GetBuffer() failed", hr);

		if(FAILED(hr = m_Direct3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView))) 
		{
			ReleaseObject(backBuffer);
			throw ApplicationException("CreateRendreTarget() failed", hr);
		}

		ReleaseObject(backBuffer);

		//Depth-Stencil Buffer--------------------------------------------
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = m_ScreenWidth;
		depthStencilDesc.Height = m_ScreenHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.SampleDesc.Count = m_IsMultiSamplingEnabled ? m_MultiSamplingCount : 1;
		depthStencilDesc.SampleDesc.Quality = m_IsMultiSamplingEnabled ? m_MultiSamplingQualityLevels - 1: 0;

		if(FAILED(hr = m_Direct3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_DepthStencilBuffer))) 
			throw ApplicationException("CreateTexture() failed", hr);

		if(FAILED(hr = m_Direct3DDevice->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView)))
			throw ApplicationException("CreateDepthStencilView() failed", hr);

		//Output Merger----------------------------------------------
		m_Direct3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		//Viewport---------------------------------------------------
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = static_cast<float>(m_ScreenWidth);
		m_Viewport.Height = static_cast<float>(m_ScreenHeight);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_Direct3DDeviceContext->RSSetViewports(1, &m_Viewport);
	}

	LRESULT WINAPI Application::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	POINT Application::centerWindow(int windowWidth, int windowHeight)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenheight = GetSystemMetrics(SM_CYSCREEN);

		POINT center;
		center.x = (screenWidth - windowWidth) / 2;
		center.y = (screenheight - windowHeight) / 2;

		return center;
	}

	void Application::shutdown()
	{
		ReleaseObject(m_RenderTargetView);
		ReleaseObject(m_DepthStencilView);
		ReleaseObject(m_SwapChain);
		ReleaseObject(m_DepthStencilBuffer);
		if(m_Direct3DDeviceContext != nullptr) m_Direct3DDeviceContext->ClearState();
		ReleaseObject(m_Direct3DDeviceContext);
		ReleaseObject(m_Direct3DDevice);
		UnregisterClass(m_WindowClass.c_str(), m_Window.hInstance);
	}

	void Application::run()
	{
		initializeWindow();
		initializeDirectX();
		initialize();

		MSG message;
		ZeroMemory(&message, sizeof(message));

		m_ApplicationClock.reset();

		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				m_ApplicationClock.updateApplicationTime(m_ApplicationTime);
				update(m_ApplicationTime);
				draw(m_ApplicationTime);
			}
		}

		shutdown();
	}
	void Application::initialize()
	{

		for(auto& component : m_Components)
		{
			component->initialize();
		}
	}
	void Application::update(const ApplicationTime& applicationTime)
	{
		for(auto& component : m_Components)
		{
			if(!component->isEnabled()) continue;

			component->update(applicationTime);
		}
	}
	void Application::draw(const ApplicationTime& applicationTime)
	{
		for(auto& component : m_Components)
		{
			if(!component->isEnabled()) continue;

			DrawableApplicationComponent* drawableComponent = component->as<DrawableApplicationComponent>();
			if(!drawableComponent || !drawableComponent->isVisible()) continue;

			drawableComponent->draw(applicationTime);
		}
	}

	void Application::exit()
	{
		PostQuitMessage(0);
	}
}