#pragma once
#include "Camera.h"

namespace Engine
{
	class Mouse;
	class Keyboard;

	class FirstPersonCamera : public Camera
	{
		RTTI_DECLARATIONS(FirstPersonCamera, Camera)

	public:
		FirstPersonCamera(Application* app)
			: Camera(app), 
			m_MouseSensitivity(s_DefaultMouseSensitivity), 
			m_RotationRate(s_DefaultRotationRate), 
			m_MovementRate(s_DefaultMovementRate) {}

		FirstPersonCamera(Application* app, float FOV, float aspectRatio, float nearPlane, float farPlane)
			: Camera(app, FOV, aspectRatio, nearPlane, farPlane),
			m_MouseSensitivity(s_DefaultMouseSensitivity), 
			m_RotationRate(s_DefaultRotationRate), 
			m_MovementRate(s_DefaultMovementRate) {}

		virtual ~FirstPersonCamera() {}

		const Keyboard* getKeyboard() const { return m_Keyboard; }
		const Mouse* getMouse() const { return m_Mouse; }

		void setKeyboard(Keyboard* keyboard) { m_Keyboard = keyboard; }
		void setMouse(Mouse* mouse) { m_Mouse = mouse; }

		float& mouseSensitivity() { return m_MouseSensitivity; }
		float& rotationRate() { return m_RotationRate; }
		float& movementrate() { return m_MovementRate; }

	protected:
		Keyboard* m_Keyboard{};
		Mouse* m_Mouse{};

		float m_MouseSensitivity = 0.0f;
		float m_RotationRate = 0.0f;
		float m_MovementRate = 0.0f;

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;

		static const float s_DefaultMouseSensitivity;
		static const float s_DefaultRotationRate;
		static const float s_DefaultMovementRate;

	private:
		FirstPersonCamera(const FirstPersonCamera&) = default;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = default;
	};
}
