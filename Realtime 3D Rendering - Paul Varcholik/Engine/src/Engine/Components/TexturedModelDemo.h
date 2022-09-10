#pragma once
#include "DrawableApplicationComponent.h"

#include "Camera.h"
#include "Engine/Core/Model.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace DirectX;

namespace Engine
{
	class TexturedModelDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(TexturedModelDemo, DrawableApplicationComponent)

	public:
		TexturedModelDemo(Application* app, Camera* camera)
			: DrawableApplicationComponent(app, camera)
		{
			XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(-5.0f, -0.5f, 2.0f) * XMMatrixIdentity());
		}

		~TexturedModelDemo();

		virtual void initialize() override;
		virtual void draw(const ApplicationTime& applicationTime) override;

		void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

	private:
		typedef struct TexturedEffectVertex
		{
			XMFLOAT4 position{};
			XMFLOAT2 texcoord{};

		 TexturedEffectVertex() {}
		 TexturedEffectVertex(XMFLOAT4 position, XMFLOAT2 texcoord) 
				: position(position), texcoord(texcoord) {}
		 TexturedEffectVertex(const TexturedEffectVertex&) = default;
		 TexturedEffectVertex& operator=(const TexturedEffectVertex&) = default;
		};

		TexturedModelDemo();
		TexturedModelDemo(const TexturedModelDemo&) = default;
		TexturedModelDemo& operator=(const TexturedModelDemo&) = default;

		UINT m_IndexCount = 0;

		ID3DX11Effect* m_Effect = nullptr;
		ID3DX11EffectTechnique* m_Technique = nullptr;
		ID3DX11EffectPass* m_Pass = nullptr;
		ID3D11InputLayout* m_InputLayout = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		ID3D11ShaderResourceView* m_TextureShaderResourceView = nullptr;

		ID3DX11EffectMatrixVariable* m_WVPVariable = nullptr;
		ID3DX11EffectShaderResourceVariable* m_ColorTextureVariable = nullptr;

		XMFLOAT4X4 m_WorldMatrix{};
	};
}
