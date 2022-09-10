#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Core/Application.h"

#include <d3d11.h>

namespace Engine
{
	class Application;

	class RenderStateHelper
	{
	public:
		RenderStateHelper(Application* app)
			: m_Application(app) {}

		virtual ~RenderStateHelper()
		{
			ReleaseObject(m_RasterizerState);
			ReleaseObject(m_BlendState);
			ReleaseObject(m_DepthStencilState);
			DeleteObjects(m_BlendFactor);
		}
		
		static void resetAll(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->RSSetState(nullptr);
			deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
			deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
		}

		ID3D11RasterizerState* getRasterizerState() { return m_RasterizerState; }
		ID3D11BlendState* getBlendState() { return m_BlendState; }
		ID3D11DepthStencilState* getDepthStencilState() { return m_DepthStencilState; }

		void saveRasterizerState()
		{
			ReleaseObject(m_RasterizerState);
			m_Application->getDirect3DDeviceContext()->RSGetState(&m_RasterizerState);
		}
		void restoreRasterizerState() const
		{
			m_Application->getDirect3DDeviceContext()->RSSetState(m_RasterizerState);
		}

		void saveBlendState()
		{
			ReleaseObject(m_BlendState);
			m_Application->getDirect3DDeviceContext()->OMGetBlendState(&m_BlendState, m_BlendFactor, &m_SampleMask);
		}
		void restoreBlendState() const
		{
			m_Application->getDirect3DDeviceContext()->OMSetBlendState(m_BlendState, m_BlendFactor, m_SampleMask);
		}

		void saveDepthStencilState()
		{
			ReleaseObject(m_DepthStencilState);
			m_Application->getDirect3DDeviceContext()->OMGetDepthStencilState(&m_DepthStencilState, &m_StencilRef);
		}
		void restoreDepthStencilState() const
		{
			m_Application->getDirect3DDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, m_StencilRef);
		}

		void saveAll()
		{
			saveBlendState();
			saveDepthStencilState();
			saveRasterizerState();
		}
		void restoreAll() const
		{
			restoreBlendState();
			restoreDepthStencilState();
			restoreRasterizerState();
		}

	private:
		RenderStateHelper(const RenderStateHelper&) = default;
		RenderStateHelper& operator=(const RenderStateHelper&) = default;

		Application* m_Application = nullptr;

		ID3D11RasterizerState* m_RasterizerState = nullptr;		
		ID3D11BlendState* m_BlendState = nullptr;
		FLOAT* m_BlendFactor = new FLOAT[4];
		UINT m_SampleMask = UINT_MAX;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
		UINT m_StencilRef = UINT_MAX;
	};
}
