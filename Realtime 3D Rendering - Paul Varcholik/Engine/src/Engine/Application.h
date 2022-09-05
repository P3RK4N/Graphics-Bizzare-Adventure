#pragma once

#include "ApplicationClock.h"
#include "ApplicationTime.h"
#include "ApplicationException.h"
#include "ApplicationComponent.h"
#include "DrawableApplicationComponent.h"

#include <windows.h>
#include <string>
#include <vector>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Engine
{
	class Application
	{
	public:
		Application(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: m_Instance(instance), m_WindowClass(windowClass), m_WindowTitle(windowTitle), m_ShowCommand(showCommand) {}
		virtual ~Application() {}

		//Window
		HINSTANCE getInstance() const { return m_Instance; }
		HWND getWindowHandle() const { return m_WindowHandle; }
		const WNDCLASSEX& getWindow() const { return m_Window; }
		const std::wstring& getWindowClass() const { return m_WindowClass; }
		const std::wstring& getWindowTitle() const { return m_WindowTitle; }

		unsigned getWidth() const { return m_ScreenWidth;  }
		unsigned getHeight() const { return m_ScreenHeight;  }
		//-------------

		//D3D11_1
		ID3D11Device1* getDirect3DDevice() const { return m_Direct3DDevice; }
		ID3D11DeviceContext1* getDirect3DDeviceContext() const { return m_Direct3DDeviceContext; }
		bool isDepthBufferEnabled() const { return m_IsDepthStencilBufferEnabled; }
		bool isFullScreen() const { return m_IsFullScreen; }
		bool isVSyncEnabled() const { return m_VSyncEnabled; }
		const D3D11_TEXTURE2D_DESC& getBackBufferDescription() const { return m_BackBufferDescription; }
		const D3D11_VIEWPORT& getViewport() const { return m_Viewport; }
		//-------------

		virtual void run();
		virtual void exit() {}
		virtual void initialize();
		virtual void update(const ApplicationTime& applicationTime);
		virtual void draw(const ApplicationTime& applicationTime);

		const std::vector<ApplicationComponent*>& getComponents() const { return m_Components; }

	protected:
		//Window
		virtual void initializeWindow();

		static const unsigned s_DefaultWidth = 1280;
		static const unsigned s_DefaultHeight = 720;

		HINSTANCE m_Instance{};
		std::wstring m_WindowClass{};
		std::wstring m_WindowTitle{};
		int m_ShowCommand;

		HWND m_WindowHandle{};
		WNDCLASSEX m_Window{};

		unsigned m_ScreenWidth = s_DefaultWidth;
		unsigned m_ScreenHeight = s_DefaultHeight;
		//---------------

		//D3D11_1
		virtual void initializeDirectX();
		
		static const UINT s_DefaultFrameRate = 144;
		static const UINT s_DefaultMultiSamplingCount = 4;

		D3D_FEATURE_LEVEL m_FeatureLevel = D3D_FEATURE_LEVEL_11_1;
		ID3D11Device1* m_Direct3DDevice = nullptr;
		ID3D11DeviceContext1* m_Direct3DDeviceContext = nullptr;
		IDXGISwapChain1* m_SwapChain = nullptr;

		UINT m_FrameRate = s_DefaultFrameRate;
		bool m_IsFullScreen = false;
		bool m_VSyncEnabled = false;
		bool m_IsDepthStencilBufferEnabled = false;
		bool m_IsMultiSamplingEnabled = false;
		UINT m_MultiSamplingCount = s_DefaultMultiSamplingCount;
		UINT m_MultiSamplingQualityLevels = 0;

		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		D3D11_TEXTURE2D_DESC m_BackBufferDescription{};
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		D3D11_VIEWPORT m_Viewport{};
		//---------------

		virtual void shutdown();

		ApplicationClock m_ApplicationClock{};
		ApplicationTime m_ApplicationTime{};
		std::vector<ApplicationComponent*> m_Components;

	private:
		Application(const Application& rhs) = default;
		Application& operator=(const Application& rhs) = default;

		//Window
		POINT centerWindow(int windowWidth, int windowHeight);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		//----------------
	};
}