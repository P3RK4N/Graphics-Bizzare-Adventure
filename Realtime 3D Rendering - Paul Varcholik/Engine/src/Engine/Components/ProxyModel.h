#pragma once
#include "DrawableApplicationComponent.h"

#include "Engine/Core/Application.h"
#include "Camera.h"
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Engine/Materials/BasicMaterial.h"

namespace Engine
{
	class Mesh;

	class ProxyModel final : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(ProxyModel, DrawableApplicationComponent)

	public:
		ProxyModel(Application* game, const std::shared_ptr<Camera>& camera, const std::string& modelFileName, float scale = 1.0f);
		ProxyModel(const ProxyModel&) = delete;
		ProxyModel(ProxyModel&&) = default;
		ProxyModel& operator=(const ProxyModel&) = delete;		
		ProxyModel& operator=(ProxyModel&&) = default;
		~ProxyModel() = default;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		bool& DisplayWireframe();

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
		void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		void SetColor(const DirectX::XMFLOAT4& color);

		virtual void initialize() override;
		virtual void update(const ApplicationTime& gameTime) override;		
		virtual void draw(const ApplicationTime& gameTime) override;

	private:
		DirectX::XMFLOAT4X4 m_WorldMatrix{};
		DirectX::XMFLOAT3 m_Position{};
		DirectX::XMFLOAT3 m_Direction{};
		DirectX::XMFLOAT3 m_Up{};
		DirectX::XMFLOAT3 m_Right{};
		BasicMaterial* m_Material;
		std::string m_ModelFileName;
		float m_Scale;
		winrt::com_ptr<ID3D11Buffer> m_VertexBuffer;
		winrt::com_ptr<ID3D11Buffer> m_IndexBuffer;		
		std::uint32_t m_IndexCount{ 0 };
		bool m_DisplayWireframe{ true };
		bool m_UpdateWorldMatrix{ true };
		bool m_UpdateMaterial{ true };
	};
}