#pragma once
#include "ApplicationComponent.h"

#include <dinput.h>

namespace Engine
{
	class Keyboard : public ApplicationComponent
	{
		RTTI_DECLARATIONS(Keyboard, ApplicationComponent)

	public:
		Keyboard(Application* app, LPDIRECTINPUT8 directInput);
		virtual ~Keyboard() override;

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;

		const byte* const getCurrentState() const;
		const byte* const getLastState() const;

		bool isKeyUp(byte key) const;
		bool isKeyDown(byte key) const;
		bool wasKeyUp(byte key) const;
		bool wasKeyDown(byte key) const;
		bool wasKeyPressedThisFrame(byte key) const;
		bool wasKeyReleasedThisFrame(byte key) const;
		bool isKeyHeldDown(byte key) const;

	private:
		Keyboard() {}
		Keyboard(const Keyboard&) = default;
		Keyboard& operator=(const Keyboard&) = default;
		
		LPDIRECTINPUT8 m_DirectInput = nullptr;
		LPDIRECTINPUTDEVICE8 m_InputDevice = nullptr;

		static const int s_KeyCount = 256;

		byte m_CurrentState[s_KeyCount]{};
		byte m_LastState[s_KeyCount]{};

	};
}