#pragma once
#include "DrawableApplicationComponent.h"

#include "Engine/Materials/ProjectionMaterial.h"
#include "DirectionalLight.h"
#include "Keyboard.h"
#include "Engine/Core/DepthMap.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class ProjectionDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(ProjectionDemo, DrawableApplicationComponent)

	public:
		ProjectionDemo(Application* app, Camera* camera, DepthMap* target, const XMMATRIX& lightViewProj, const XMFLOAT3& pos, bool first = true)
			: DrawableApplicationComponent(app, camera), m_DepthMap(target), m_First(first) 
		{
			XMStoreFloat4x4(&m_LightViewProj, lightViewProj);
			//-2.5f, 1.0f, -5.0f
			XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranslation(pos.x, pos.y, pos.z) * XMMatrixIdentity());
		}
		~ProjectionDemo();

		const XMVECTOR& getAmbientColor() const { return XMLoadColor(&m_AmbientColor); }
		const XMFLOAT3& getLightPosition() const { return m_LightPosition; }

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		ProjectionDemo() = default;
		ProjectionDemo(const ProjectionDemo&) = default;
		ProjectionDemo& operator=(const ProjectionDemo&) = default;

		void updateAmbientLight(const ApplicationTime& applicationTime);
		void updateDirectionalLight(const ApplicationTime& applicationTime);

		static const float s_AmbientModulationRate;
		static const XMFLOAT2 s_RotationRate;

		Effect* m_Effect = nullptr;										
		ProjectionMaterial* m_Material = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		UINT m_IndexCount = 0;

		Keyboard* m_Keyboard = nullptr;

		ID3D11ShaderResourceView* m_TextureShaderResourceView = nullptr;
		DepthMap* m_DepthMap;

		XMCOLOR m_AmbientColor{};
		XMCOLOR m_SpecularColor{};
		float m_SpecularPower = 0.0f;

		DirectionalLight* m_DirectionalLight = nullptr;
		XMFLOAT3 m_LightPosition{};
		float m_LightRadius = 0.0f;

		XMFLOAT4X4 m_WorldMatrix{};
		XMFLOAT4X4 m_LightViewProj{};

		bool m_First;
	};
}