#pragma once
#include "DrawableApplicationComponent.h"
#include "Engine/Materials/Material.h"

#include <functional>

namespace Engine
{
	struct PositionTextureVertex
	{
		XMFLOAT4 position;
		XMFLOAT2 texturecoordinate;

		PositionTextureVertex& operator=(const PositionTextureVertex&) = default;
		PositionTextureVertex(const PositionTextureVertex&) = default;
		PositionTextureVertex(const XMFLOAT4& pos, const XMFLOAT2 texcoord) 
			: position(pos), texturecoordinate(texcoord) {}
	};

	class FullScreenQuad : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(FullScreenQuad, DrawableApplicationComponent)

	public:
		FullScreenQuad(Application* app);
		FullScreenQuad(Application* app, Material* material);
		~FullScreenQuad();

		Material* getMaterial() { return m_Material; }
		void setMaterial(Material* material, const std::string& techniqueName, const std::string& passName);
		void setActiveTechnique(const std::string& techniqueName, const std::string& passName);
		void setCustomUpdateMaterial(std::function<void()> callback);

		virtual void initialize() override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		FullScreenQuad();
		FullScreenQuad(const FullScreenQuad&) = default;
		FullScreenQuad& operator=(const FullScreenQuad&) = default;

		Material* m_Material = nullptr;				
		Pass* m_Pass = nullptr;
		ID3D11InputLayout* m_InputLayout = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		UINT m_IndexCount = 0;
		std::function<void()> m_CustomUpdateMaterial{};
	};
}