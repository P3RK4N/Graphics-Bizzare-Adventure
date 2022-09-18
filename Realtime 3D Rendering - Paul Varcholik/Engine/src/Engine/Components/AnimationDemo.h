#pragma once
#include "DrawableApplicationComponent.h"

#include "Engine/Materials/AnimationMaterial.h"
#include "Engine/Materials/DiffuseMaterial.h"
#include "DirectionalLight.h"
#include "Keyboard.h"

#include "AnimationPlayer.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class AnimationDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(AnimationDemo, DrawableApplicationComponent)

	public:
		AnimationDemo(Application* app, Camera* camera)
			: DrawableApplicationComponent(app, camera) 
		{
		}
		~AnimationDemo();

		const XMVECTOR& getAmbientColor() const { return XMLoadColor(&m_AmbientColor); }
		const XMFLOAT3& getLightPosition() const { return m_LightPosition; }

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		AnimationDemo() = default;
		AnimationDemo(const AnimationDemo&) = default;
		AnimationDemo& operator=(const AnimationDemo&) = default;

		void updateAmbientLight(const ApplicationTime& applicationTime);
		void updateDirectionalLight(const ApplicationTime& applicationTime);

		static const float s_AmbientModulationRate;
		static const XMFLOAT2 s_RotationRate;

		Effect* m_Effect = nullptr;										
		AnimationMaterial* m_Material = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		UINT m_IndexCount = 0;

		Keyboard* m_Keyboard = nullptr;

		ID3D11ShaderResourceView* m_TextureShaderResourceView = nullptr;
		XMCOLOR m_AmbientColor{};
		XMCOLOR m_SpecularColor{};
		float m_SpecularPower = 0.0f;

		DirectionalLight* m_DirectionalLight = nullptr;
		XMFLOAT3 m_LightPosition{};
		float m_LightRadius = 0.0f;

		XMFLOAT4X4 m_WorldMatrix{};

		AnimationPlayer* m_AnimationPlayer = nullptr;

		Model* m_Model;
	};
}