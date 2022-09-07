#include "pch.h"
#include "FirstPersonCamera.h"

#include "Mouse.h"
#include "Keyboard.h"

using namespace DirectX;

namespace Engine
{
	RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::s_DefaultRotationRate = DirectX::XMConvertToRadians(10.0f);
	const float FirstPersonCamera::s_DefaultMouseSensitivity = 5.0f;
	const float FirstPersonCamera::s_DefaultMovementRate = 10.0f;

	void FirstPersonCamera::initialize()
	{
		m_Keyboard = (Keyboard*)m_Application->getServiceContainer().getService(Keyboard::typeIdClass());
		m_Mouse = (Mouse*)m_Application->getServiceContainer().getService(Mouse::typeIdClass());

		Camera::initialize();
	}

	void FirstPersonCamera::update(const ApplicationTime& applicationTime)
	{
		if(!m_Keyboard && !m_Mouse)
			return;

		float elapsedTime = (float)applicationTime.getElapsedApplicationTime();
		DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&m_Right);
		DirectX::XMVECTOR forward = DirectX::XMLoadFloat3(&m_Forward);
		
		if(m_Keyboard)
		{
			DirectX::XMFLOAT2 movementAmount = Vector2Helper::zero;
			float totalFactor = m_MovementRate * elapsedTime;

			if(m_Keyboard->isKeyDown(DIK_W)) movementAmount.y += totalFactor;
			if(m_Keyboard->isKeyDown(DIK_S)) movementAmount.y -= totalFactor;
			if(m_Keyboard->isKeyDown(DIK_A)) movementAmount.x -= totalFactor;
			if(m_Keyboard->isKeyDown(DIK_D)) movementAmount.x += totalFactor;

			DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_Position);
			DirectX::XMVECTOR movement = DirectX::XMLoadFloat2(&movementAmount);

			position += right * DirectX::XMVectorGetX(movement);
			position += forward * DirectX::XMVectorGetY(movement);

			DirectX::XMStoreFloat3(&m_Position, position);
		}
		
		if(m_Mouse)
		{
			DirectX::XMFLOAT2 rotationAmount = Vector2Helper::zero;
			
			if(m_Mouse->isMouseButtonHeldDown(MouseButtonsLeft))
			{
				LPDIMOUSESTATE mouseState = m_Mouse->getCurrentState();

				float totalFactor = elapsedTime * m_MouseSensitivity * m_RotationRate;

				rotationAmount.x = -mouseState->lX * totalFactor;
				rotationAmount.y = -mouseState->lY * totalFactor;

				DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat2(&rotationAmount);
				DirectX::XMMATRIX rotationMatrix =
					DirectX::XMMatrixMultiply
					(
						DirectX::XMMatrixRotationAxis(right, DirectX::XMVectorGetY(rotationVector)),
						DirectX::XMMatrixRotationY(DirectX::XMVectorGetX(rotationVector))
					);

				applyRotation(rotationMatrix);
			}
		}
		
		Camera::update(applicationTime);
	}
}