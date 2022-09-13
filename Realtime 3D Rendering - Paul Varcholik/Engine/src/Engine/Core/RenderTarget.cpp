#include "pch.h"
#include "RenderTarget.h"

namespace Engine
{
	static HRESULT hr1;

	RTTI_DEFINITIONS(RenderTarget)

	std::stack<RenderTarget::RenderTargetData> RenderTarget::s_RenderTargetStack{};

	void RenderTarget::begin(ID3D11DeviceContext* deviceContext, UINT viewCount, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView, const D3D11_VIEWPORT& viewport)
	{
		s_RenderTargetStack.push(RenderTargetData(viewCount, renderTargetViews, depthStencilView, viewport));

		deviceContext->OMSetRenderTargets(viewCount, renderTargetViews, depthStencilView);
		deviceContext->RSSetViewports(1, &viewport);
	}

	void RenderTarget::end(ID3D11DeviceContext* deviceContext)
	{
		s_RenderTargetStack.pop();
		if(s_RenderTargetStack.size())
		{
			RenderTargetData rtd = s_RenderTargetStack.top();
			deviceContext->OMSetRenderTargets(rtd.viewCount, rtd.renderTargetViews, rtd.depthStencilView);
			deviceContext->RSSetViewports(1, &rtd.viewport);
		}
		else
		{
			static ID3D11RenderTargetView* null = nullptr;
			deviceContext->OMSetRenderTargets(1, &null, nullptr);
		}
	}
}
