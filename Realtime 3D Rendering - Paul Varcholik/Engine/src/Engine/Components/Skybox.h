#pragma once
#include "DrawableApplicationComponent.h"

#include "Engine/Core/SkyboxMaterial.h"

namespace Engine
{
	class Skybox : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(Skybox, DrawableApplicationComponent)

	public:
		Skybox(Application* app, Camera* camera, const std::wstring& cubeMapFileName, float scale);
		~Skybox();

		virtual void initialize() override;
		virtual void update(const ApplicationTime& gameTime) override;
		virtual void draw(const ApplicationTime& gameTime) override;

	private:
		Skybox();
		Skybox(const Skybox& rhs);
		Skybox& operator=(const Skybox& rhs);

		std::wstring m_CubeMapFileName{};
		Effect* m_Effect = nullptr;										
		SkyboxMaterial* m_Material = nullptr;
		ID3D11ShaderResourceView* m_CubeMapShaderResourceView = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;								
		UINT m_IndexCount = 0;

		XMFLOAT4X4 m_WorldMatrix{};
		XMFLOAT4X4 m_ScaleMatrix{};
	};
}