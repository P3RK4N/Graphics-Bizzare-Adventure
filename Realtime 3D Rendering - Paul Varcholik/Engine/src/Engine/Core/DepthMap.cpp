#include "pch.h"
#include "DepthMap.h"

namespace Engine
{
	static HRESULT hr1;

	RTTI_DEFINITIONS(DepthMap)

	DepthMap::DepthMap(Application* app, UINT width, UINT height)
		: RenderTarget(), m_Application(app)
	{
		init(width, height);
	}

	void DepthMap::init(UINT width, UINT height)
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* depthStencilTexture = nullptr;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture)))
			throw new ApplicationException();

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		ZeroMemory(&resourceViewDesc, sizeof(resourceViewDesc));
		resourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = 1;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateShaderResourceView(depthStencilTexture, &resourceViewDesc, &m_OutputDepth)))
		{
			ReleaseObject(depthStencilTexture);
			throw new ApplicationException();
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &m_DepthStencilView)))
		{
			ReleaseObject(depthStencilTexture);
			throw new ApplicationException("", hr1);
		}

		ReleaseObject(depthStencilTexture);

		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
	}

	DepthMap::~DepthMap()
	{
		ReleaseObject(m_OutputDepth);
		ReleaseObject(m_DepthStencilView);
	}

	void DepthMap::begin()
	{
		static ID3D11RenderTargetView* null = nullptr;
		m_Application->getDirect3DDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		RenderTarget::begin(m_Application->getDirect3DDeviceContext(), 1, &null, m_DepthStencilView, m_Viewport);
	}

	void DepthMap::end()
	{
		ID3D11DeviceContext* deviceContext = m_Application->getDirect3DDeviceContext();
		ID3D11RenderTargetView* renderTarget = m_Application->getRenderTargetView();
		D3D11_VIEWPORT viewport = m_Application->getViewport();

		RenderTarget::end(deviceContext);

		deviceContext->OMSetRenderTargets(1, &renderTarget, m_Application->getDepthStencilView());
		deviceContext->RSSetViewports(1, &viewport);
	}
}
