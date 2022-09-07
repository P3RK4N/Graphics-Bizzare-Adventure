#include "pch.h"
#include "Camera.h"

namespace Engine
{

	RTTI_DEFINITIONS(Camera);

	const float Camera::s_DefaultFOV = DirectX::XM_PIDIV4;
	const float Camera::s_DefaultNearPlane = 0.01f;
	const float Camera::s_DefaultFarPlane = 1000.0f;
	const float Camera::s_DefaultAspectRatio = 1600.0f / 900.0f;
	
	void Camera::reset()
	{
		m_Position = Vector3Helper::zero;
		m_Forward = Vector3Helper::forward;
		m_Up = Vector3Helper::up;
		m_Right = Vector3Helper::right;

		updateViewMatrix();
	}

	void Camera::initialize()
	{
		updateProjectionMatrix();
		reset();
	}

	void Camera::update(const ApplicationTime& applicationTime)
	{
		updateViewMatrix();
	}

	void Camera::updateViewMatrix()
	{
		DirectX::XMVECTOR eyePosition = DirectX::XMLoadFloat3(&m_Position);
		DirectX::XMVECTOR forward = DirectX::XMLoadFloat3(&m_Forward);
		DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_Up);
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookToRH(eyePosition, forward, up);
		DirectX::XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
	}

	void Camera::updateProjectionMatrix()
	{
		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
	}

	void Camera::applyRotation(DirectX::CXMMATRIX transform)
	{
		DirectX::XMVECTOR forward = DirectX::XMLoadFloat3(&m_Forward);
		DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_Up);

		forward = DirectX::XMVector3TransformNormal(forward, transform);
		up = DirectX::XMVector3TransformNormal(up, transform);

		forward = DirectX::XMVector3Normalize(forward);
		up = DirectX::XMVector3Normalize(up);

		DirectX::XMVECTOR right = DirectX::XMVector3Cross(forward, up);

		up = DirectX::XMVector3Cross(right, forward);

		DirectX::XMStoreFloat3(&m_Forward, forward);
		DirectX::XMStoreFloat3(&m_Up, up);
		DirectX::XMStoreFloat3(&m_Right, right);
	}

	void Camera::applyRotation(const DirectX::XMFLOAT4X4& transform)
	{
		applyRotation(DirectX::XMLoadFloat4x4(&transform));
	}
}