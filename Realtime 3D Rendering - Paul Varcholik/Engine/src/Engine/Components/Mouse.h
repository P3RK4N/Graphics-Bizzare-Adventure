#pragma once
#include "ApplicationComponent.h"

#include <dinput.h>

namespace Engine
{
	enum MouseButtons
	{
		MouseButtonsLeft = 0,
		MouseButtonsRight = 1,
		MouseButtonsMiddle = 2,
		MouseButtonsX1 = 3
	};

	class Mouse : public ApplicationComponent
	{
		RTTI_DECLARATIONS(Mouse, ApplicationComponent)

	public:
		Mouse(Application* app, LPDIRECTINPUT8 directInput);
		virtual ~Mouse() override;

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;

		const LPDIMOUSESTATE const getCurrentState() const { return (LPDIMOUSESTATE)&m_CurrentState; }
		const LPDIMOUSESTATE const getLastState() const { return (LPDIMOUSESTATE)&m_LastState; }

		long getX() const { return m_X; }
		long getY() const { return m_Y; }
		long getScroll() const { return m_Scroll; }

		bool isButtonUp(MouseButtons button) const { return (m_CurrentState.rgbButtons[button] & 0x80) == 0; }
		bool isButtonDown(MouseButtons button) const { return !isButtonUp(button); }
		bool wasButtonUp(MouseButtons button) const { return (m_LastState.rgbButtons[button] & 0x80) == 0; }
		bool wasButtonDown(MouseButtons button) const { return !wasButtonUp(button); }
		bool wasButtonPressedThisFrame(MouseButtons button) const { return isButtonDown(button) && wasButtonUp(button); }
		bool wasButtonReleasedThisFrame(MouseButtons button) const { return isButtonUp(button) && wasButtonDown(button); }
		bool isMouseButtonHeldDown(MouseButtons button) const { return isButtonDown(button) && wasButtonDown(button); }

	private:
		Mouse() {}
		Mouse(const Mouse&) = default;
		Mouse& operator=(const Mouse&) = default;

		LPDIRECTINPUT8 m_DirectInput = nullptr;
		LPDIRECTINPUTDEVICE8 m_InputDevice = nullptr;
		
		DIMOUSESTATE m_CurrentState{};
		DIMOUSESTATE m_LastState{};

		long m_X = 0;
		long m_Y = 0;
		long m_Scroll = 0;
	};
}