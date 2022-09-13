#include "pch.h"
#include "FullScreenRenderTarget.h"

namespace Engine
{
	static HRESULT hr1;

	FullScreenRenderTarget::FullScreenRenderTarget(Application* app)
		: m_Application(app)
	{
		D3D11_TEXTURE2D_DESC fullScreenTextureDesc;
		ZeroMemory(&fullScreenTextureDesc, sizeof(fullScreenTextureDesc));

		fullScreenTextureDesc.Width = app->getWidth();
		fullScreenTextureDesc.Height = app->getHeight();
		fullScreenTextureDesc.MipLevels = 1;
		fullScreenTextureDesc.ArraySize = 1;
		fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		fullScreenTextureDesc.SampleDesc.Count = 1;
		fullScreenTextureDesc.SampleDesc.Quality = 0;
		fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* fullScreenTexture = nullptr;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateTexture2D(&fullScreenTextureDesc, nullptr, &fullScreenTexture)))
			throw new ApplicationException();

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateShaderResourceView(fullScreenTexture, nullptr, &m_OutputColor)))
			throw new ApplicationException();

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateRenderTargetView(fullScreenTexture, nullptr, &m_RenderTargetView)))
		{
			ReleaseObject(fullScreenTexture);
			throw new ApplicationException();
		}

		ReleaseObject(fullScreenTexture);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.Width = app->getWidth();
		depthStencilDesc.Height = app->getHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* depthStencilBuffer = nullptr;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer)))
			throw new ApplicationException();

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateDepthStencilView(depthStencilBuffer, nullptr, &m_DepthStencilView)))
		{
			ReleaseObject(depthStencilBuffer);
			throw new ApplicationException("", hr1);
		}

		ReleaseObject(depthStencilBuffer);
	}

	FullScreenRenderTarget::~FullScreenRenderTarget()
	{
		ReleaseObject(m_OutputColor);
		ReleaseObject(m_DepthStencilView);
		ReleaseObject(m_RenderTargetView);
	}

	void FullScreenRenderTarget::begin()
	{
		m_Application->getDirect3DDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	}

	void FullScreenRenderTarget::end()
	{
		m_Application->resetRenderTargets();
	}
}
