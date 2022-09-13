#pragma once
#include "Application.h"



namespace Engine
{
	class FullScreenRenderTarget
	{
	public:
		FullScreenRenderTarget(Application* app);
		~FullScreenRenderTarget();

		ID3D11ShaderResourceView* getOutputColor() const { return m_OutputColor; }
		ID3D11RenderTargetView* getRenderTargetView() const { return m_RenderTargetView; }
		ID3D11DepthStencilView* getDepthStencilView() const { return m_DepthStencilView; }

		void begin();
		void end();

	private:
		FullScreenRenderTarget() = default;
		FullScreenRenderTarget(const FullScreenRenderTarget&) = default;
		FullScreenRenderTarget& operator=(const FullScreenRenderTarget&) = default;

		Application* m_Application = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11ShaderResourceView* m_OutputColor = nullptr;	
	};
}