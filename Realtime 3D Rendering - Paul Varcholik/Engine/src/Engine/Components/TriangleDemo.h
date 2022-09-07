#pragma once
#include "DrawableApplicationComponent.h"

#include "Camera.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace DirectX;

namespace Engine
{
	class TriangleDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(TriangleDemo, DrawableApplicationComponent)

	public:
		TriangleDemo(Application* app, Camera* camera)
			: DrawableApplicationComponent(app, camera)
		{
			XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(0.0f, -0.5f, 2.0f) * XMMatrixIdentity());
		}

		~TriangleDemo();

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

		TriangleDemo();
		TriangleDemo(const TriangleDemo&) = default;
		TriangleDemo& operator=(const TriangleDemo&) = default;

		ID3DX11Effect* m_Effect = nullptr;
		ID3DX11EffectTechnique* m_Technique = nullptr;
		ID3DX11EffectPass* m_Pass = nullptr;
		ID3DX11EffectMatrixVariable* m_WVPVariable = nullptr;
		ID3D11InputLayout* m_InputLayout = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;

		XMFLOAT4X4 m_WorldMatrix{};
	};
}
