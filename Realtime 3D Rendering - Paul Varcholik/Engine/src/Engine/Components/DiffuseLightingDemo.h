#pragma once
#include "DrawableApplicationComponent.h"

#include "Engine/Materials/DiffuseMaterial.h"
#include "DirectionalLight.h"
#include "Keyboard.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class DiffuseLightingDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(DiffuseLightingDemo, DrawableApplicationComponent)

	public:
		DiffuseLightingDemo(Application* app, Camera* camera)
			: DrawableApplicationComponent(app, camera) 
		{
			XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(5.0f, 0.0f, -5.0f) * XMMatrixIdentity());
		}
		~DiffuseLightingDemo();

		const XMVECTOR& getAmbientColor() const { return XMLoadColor(&m_AmbientColor); }

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		DiffuseLightingDemo() = default;
		DiffuseLightingDemo(const DiffuseLightingDemo&) = default;
		DiffuseLightingDemo& operator=(const DiffuseLightingDemo&) = default;

		void updateAmbientLight(const ApplicationTime& applicationTime);
		void updateDirectionalLight(const ApplicationTime& applicationTime);

		static const float s_AmbientModulationRate;
		static const XMFLOAT2 s_RotationRate;

		Effect* m_Effect = nullptr;										
		DiffuseLightingMaterial* m_Material = nullptr;
		ID3D11ShaderResourceView* m_TextureShaderResourceView = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		UINT m_IndexCount = 0;
		XMCOLOR m_AmbientColor{};
		DirectionalLight* m_DirectionalLight = nullptr;

		Keyboard* m_Keyboard = nullptr;
		XMFLOAT4X4 m_WorldMatrix{};
	};
}