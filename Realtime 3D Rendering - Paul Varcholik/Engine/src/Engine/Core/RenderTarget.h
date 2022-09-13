#pragma once
#include "Application.h"

#include <stack>

namespace Engine
{
	class RenderTarget : public RTTI
	{
		RTTI_DECLARATIONS(RenderTarget, RTTI)

	public:
		RenderTarget() {}
		virtual ~RenderTarget() {}

		virtual void begin() = 0;
		virtual void end() = 0;

	protected:
		struct RenderTargetData
		{
			UINT viewCount;
			ID3D11RenderTargetView** renderTargetViews;
			ID3D11DepthStencilView* depthStencilView;
			D3D11_VIEWPORT viewport;

			RenderTargetData(UINT viewCount, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView, const D3D11_VIEWPORT& viewport)
				: viewCount(viewCount), renderTargetViews(renderTargetViews), depthStencilView(depthStencilView), viewport(viewport) { }
		};

		void begin
		(
			ID3D11DeviceContext* deviceContext,
			UINT viewCount,
			ID3D11RenderTargetView** renderTargetViews,
			ID3D11DepthStencilView* depthStencilView,
			const D3D11_VIEWPORT& viewport
		);

		void end(ID3D11DeviceContext* deviceContext);

	private:
		RenderTarget(const RenderTarget&) = default;
		RenderTarget& operator=(const RenderTarget&) = default;


		static std::stack<RenderTargetData> s_RenderTargetStack;
	};
}