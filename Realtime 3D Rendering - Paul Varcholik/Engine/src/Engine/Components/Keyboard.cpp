#include "pch.h"
#include "Keyboard.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(Keyboard)

	Keyboard::Keyboard(Application* app, LPDIRECTINPUT8 directInput)
		: ApplicationComponent(app), m_DirectInput(directInput)
	{
		assert(m_DirectInput != nullptr);
		ZeroMemory(m_CurrentState, sizeof(m_CurrentState));
		ZeroMemory(m_LastState, sizeof(m_LastState));
	}

	Keyboard::~Keyboard()
	{
		if(m_InputDevice != nullptr)
		{
			m_InputDevice->Unacquire();
			ReleaseObject(m_InputDevice);
		}
	}

	const byte* const Keyboard::getCurrentState() const
	{
		return m_CurrentState;
	}

	const byte* const Keyboard::getLastState() const
	{
		return m_LastState;
	}

	void Keyboard::initialize()
	{
		if(!m_Application)
			throw new ApplicationException("m_Application is nullptr");

		if(FAILED(hr = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_InputDevice, nullptr)))
			throw new ApplicationException("CreateDevice() error", hr);

		if(FAILED(hr = m_InputDevice->SetDataFormat(&c_dfDIKeyboard)))
			throw new ApplicationException("SetDataFormat() error", hr);

		if(FAILED(hr = m_InputDevice->SetCooperativeLevel(m_Application->getWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			throw new ApplicationException("SetCooperativeLevel() error", hr);

		if(FAILED(hr = m_InputDevice->Acquire()))
			throw new ApplicationException("Acquire() error", hr);

	}

	void Keyboard::update(const ApplicationTime& applicationTime)
	{
		if(m_InputDevice != nullptr)
		{
			memcpy(m_LastState, m_CurrentState, sizeof(m_CurrentState));

			if(FAILED(hr = m_InputDevice->GetDeviceState(sizeof(m_CurrentState), (LPVOID)m_CurrentState)))
			{
				if(SUCCEEDED(m_InputDevice->Acquire()))
				{
					m_InputDevice->GetDeviceState(sizeof(m_CurrentState), (LPVOID)m_CurrentState);
				}
			}
		}
	}

	bool Keyboard::isKeyUp(byte key) const
	{
		return (m_CurrentState[key] & 0x80) == 0;
	}

	bool Keyboard::isKeyDown(byte key) const
	{
		return !isKeyUp(key);
	}

	bool Keyboard::wasKeyUp(byte key) const
	{
		return (m_LastState[key] & 0x80) == 0;
	}

	bool Keyboard::wasKeyDown(byte key) const
	{
		return !wasKeyUp(key);
	}

	bool Keyboard::wasKeyPressedThisFrame(byte key) const
	{
		return isKeyDown(key) && wasKeyUp(key);
	}

	bool Keyboard::wasKeyReleasedThisFrame(byte key) const
	{
		return wasKeyDown(key) && isKeyUp(key);
	}

	bool Keyboard::isKeyHeldDown(byte key) const
	{
		return wasKeyDown(key) && isKeyDown(key);
	}
}
