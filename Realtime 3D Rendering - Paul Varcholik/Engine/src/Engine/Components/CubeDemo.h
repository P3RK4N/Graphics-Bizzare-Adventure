#pragma once
#include "DrawableApplicationComponent.h"

#include "Camera.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace DirectX;

namespace Engine
{
	class CubeDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(CubeDemo, DrawableApplicationComponent)

	public:
		CubeDemo(Application* app, Camera* camera)
			: DrawableApplicationComponent(app, camera)
		{
			XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(3.0f, -0.5f, 2.0f) * XMMatrixIdentity());
		}

		~CubeDemo();

		virtual void initialize() override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		typedef struct BasicEffectVertex
		{
			XMFLOAT4 position{};
			XMFLOAT4 color{};

			BasicEffectVertex() {}
			BasicEffectVertex(XMFLOAT4 position, XMFLOAT4 color) 
				: position(position), color(color) {}
			BasicEffectVertex(const BasicEffectVertex&) = default;
			BasicEffectVertex& operator=(const BasicEffectVertex&) = default;
		};

		CubeDemo();
		CubeDemo(const CubeDemo&) = default;
		CubeDemo& operator=(const CubeDemo&) = default;

		ID3DX11Effect* m_Effect = nullptr;
		ID3DX11EffectTechnique* m_Technique = nullptr;
		ID3DX11EffectPass* m_Pass = nullptr;
		ID3DX11EffectMatrixVariable* m_WVPVariable = nullptr;
		ID3D11InputLayout* m_InputLayout = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;

		XMFLOAT4X4 m_WorldMatrix{};
	};
}
