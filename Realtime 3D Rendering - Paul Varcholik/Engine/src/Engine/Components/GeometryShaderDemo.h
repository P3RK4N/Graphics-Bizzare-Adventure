#pragma once
#include "DrawableApplicationComponent.h"
#include "Engine/Components/Keyboard.h"

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class Effect;
	class BillboardingMaterial;

	class GeometryMaterialDemo : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(GeometryMaterialDemo, DrawableApplicationComponent)

	public:
		GeometryMaterialDemo(Application* app, Camera* camera, float position = 0.0f, std::string image = "displacement1.jpg");
		~GeometryMaterialDemo();

		virtual void initialize() override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void update(const ApplicationTime& applicationTime) override;

		float m_SubdivisionFactor = 1.0f;
		float m_SubdivisionStrength = 0.0f;

	private:
		GeometryMaterialDemo() = default;
		GeometryMaterialDemo(const GeometryMaterialDemo&) = default;
		GeometryMaterialDemo& operator=(const GeometryMaterialDemo&) = default;

		float m_Position = 0.0f;
		std::string m_Image{};

		Keyboard* m_Keyboard = nullptr;

		Effect* m_GeometryEffect = nullptr;					
		BillboardingMaterial* m_BillboardingMaterial = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;

		ID3D11ShaderResourceView* m_ColorTextureView = nullptr;
		XMFLOAT4X4 m_TextureMatrix{};
	};
}