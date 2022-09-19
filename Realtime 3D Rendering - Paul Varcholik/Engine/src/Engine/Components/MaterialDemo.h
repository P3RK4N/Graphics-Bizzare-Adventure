#pragma once
#include "DrawableApplicationComponent.h"

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class Effect;
	class BasicMaterial;

	class MaterialDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(MaterialDemo, DrawableApplicationComponent)

	public:
		MaterialDemo(Application* app, Camera* camera);
		~MaterialDemo();

		virtual void initialize() override;
		virtual void draw(const ApplicationTime& applicationTime) override;
	
	private:
		MaterialDemo() = default;
		MaterialDemo(const MaterialDemo&) = default;
		MaterialDemo& operator=(const MaterialDemo&) = default;

		Effect* m_GeometryEffect = nullptr;					
		BasicMaterial* m_BillboardingMaterial = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		UINT m_IndexCount = 0;

		XMFLOAT4X4 m_WorldMatrix{};
	};
}