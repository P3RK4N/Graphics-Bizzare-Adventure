#pragma once
#include "ApplicationComponent.h"
#include "Engine/Core/Application.h"

#include <Windows.h>
#include <DirectXMath.h>

namespace Engine
{
	class Camera : public ApplicationComponent
	{
		RTTI_DECLARATIONS(Camera, ApplicationComponent)

	public:
		Camera(Application* app)
			: ApplicationComponent(app), 
			m_FOV(s_DefaultFOV), 
			m_AspectRatio(m_Application->getAspectRatio()), 
			m_NearPlane(s_DefaultNearPlane), 
			m_FarPlane(s_DefaultFarPlane) {}
		
		Camera(Application* app, float FOV, float aspectRatio, float nearPlane, float farPlane)
			: ApplicationComponent(app), 
			m_FOV(FOV), 
			m_AspectRatio(aspectRatio), 
			m_NearPlane(nearPlane), 
			m_FarPlane(farPlane) 
		{ updateProjectionMatrix(); }
		
		virtual ~Camera() {}

		const DirectX::XMFLOAT3& getPosition() const { return m_Position; }
		const DirectX::XMFLOAT3& getForward() const { return m_Forward; }
		const DirectX::XMFLOAT3& getUp() const { return m_Up; }
		const DirectX::XMFLOAT3& getRight() const { return m_Right; }

		DirectX::XMVECTOR getPositionVector() const { return DirectX::XMLoadFloat3(&m_Position); }
		DirectX::XMVECTOR getForwardVector() const { return DirectX::XMLoadFloat3(&m_Forward); }
		DirectX::XMVECTOR getUpVector() const { return DirectX::XMLoadFloat3(&m_Up); }
		DirectX::XMVECTOR getRightVector() const { return DirectX::XMLoadFloat3(&m_Right); }

		float getAspectRatio() const { return m_AspectRatio; }
		float getFOV() const { return m_FOV; }
		float getNearPlane() const { return m_NearPlane; }
		float getFarPlane() const { return m_FarPlane; }

		DirectX::XMMATRIX getViewMatrix() const { return DirectX::XMLoadFloat4x4(&m_ViewMatrix); }
		DirectX::XMMATRIX getProjectionMatrix() const { return DirectX::XMLoadFloat4x4(&m_ProjectionMatrix); }
		DirectX::XMMATRIX getViewProjectionMatrix() const { return DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&m_ViewMatrix), DirectX::XMLoadFloat4x4(&m_ProjectionMatrix)); }

		virtual void setPosition(FLOAT x, FLOAT y, FLOAT z) { m_Position = { x, y, z }; }
		virtual void setPosition(DirectX::FXMVECTOR position) { DirectX::XMStoreFloat3(&m_Position, position); }
		virtual void setPosition(const DirectX::XMFLOAT3& position) { m_Position = position; }

		void applyRotation(DirectX::CXMMATRIX transform);
		void applyRotation(const DirectX::XMFLOAT4X4& transform);

		virtual void updateViewMatrix();
		virtual void updateProjectionMatrix();

		virtual void reset();
		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;

		static const float s_DefaultFOV;
		static const float s_DefaultAspectRatio;
		static const float s_DefaultNearPlane;
		static const float s_DefaultFarPlane;

	protected:
		float m_FOV = 0.0f;
		float m_AspectRatio = 0.0f;
		float m_NearPlane = 0.0f;
		float m_FarPlane = 0.0f;

		DirectX::XMFLOAT3 m_Position{};
		DirectX::XMFLOAT3 m_Forward{};
		DirectX::XMFLOAT3 m_Up{};
		DirectX::XMFLOAT3 m_Right{};

		DirectX::XMFLOAT4X4 m_ViewMatrix{};
		DirectX::XMFLOAT4X4 m_ProjectionMatrix{};

	private:
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
	};
}