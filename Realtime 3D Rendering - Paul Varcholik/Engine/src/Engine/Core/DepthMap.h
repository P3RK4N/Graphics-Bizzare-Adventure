#pragma once
#include "RenderTarget.h"

namespace Engine
{
	class DepthMap : public RenderTarget
	{
		RTTI_DECLARATIONS(DepthMap, RenderTarget)

	public:
		DepthMap(Application* app, UINT width, UINT height);
		~DepthMap();

		ID3D11ShaderResourceView* getOutputDepth() { return m_OutputDepth; }
		ID3D11DepthStencilView* getDepthStencilView() { return m_DepthStencilView; }

		virtual void begin() override;
		virtual void end() override;

	private:
		DepthMap();
		DepthMap(const DepthMap&) = default;
		DepthMap& operator=(const DepthMap&) = default;

		void init(UINT width, UINT height);

		Application* m_Application = nullptr;						
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11ShaderResourceView* m_OutputDepth = nullptr;
		D3D11_VIEWPORT m_Viewport;
	};

}