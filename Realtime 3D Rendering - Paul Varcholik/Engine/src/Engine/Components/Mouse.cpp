#include "pch.h"
#include "Mouse.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	namespace Utility
	{
		bool isMouseHovering(LPRECT rect, LPPOINT point)
		{
			return point->x >= rect->left && point->x <= rect->right && point->y >= rect->top && point->y <= rect->bottom;
		}
	}

	static HRESULT hr;

	RTTI_DEFINITIONS(Mouse)

	Mouse::Mouse(Application* app, LPDIRECTINPUT8 directInput)
		: ApplicationComponent(app), m_DirectInput(directInput)
	{
		assert(m_DirectInput != nullptr);
		ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
		ZeroMemory(&m_LastState, sizeof(m_LastState));

		POINT mousePos{};
		GetCursorPos(&mousePos);
		m_X = mousePos.x;
		m_Y = mousePos.y;
	}

	Mouse::~Mouse()
	{
		if(m_InputDevice != nullptr)
		{
			m_InputDevice->Unacquire();
			ReleaseObject(m_InputDevice);
		}
	}

	void Mouse::initialize()
	{
		if(!m_Application)
			throw new ApplicationException("m_Application is nullptr");

		if(FAILED(hr = m_DirectInput->CreateDevice(GUID_SysMouse, &m_InputDevice, nullptr)))
			throw new ApplicationException("CreateDevice() error", hr);

		if(FAILED(hr = m_InputDevice->SetDataFormat(&c_dfDIMouse)))
			throw new ApplicationException("SetDataFormat() error", hr);

		if(FAILED(hr = m_InputDevice->SetCooperativeLevel(m_Application->getWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			throw new ApplicationException("SetCooperativeLevel() error", hr);

		if(FAILED(hr = m_InputDevice->Acquire()))
			throw new ApplicationException("Acquire() error", hr);

	}

	void Mouse::update(const ApplicationTime& applicationTime)
	{
		if(m_InputDevice != nullptr)
		{
			memcpy(&m_LastState, &m_CurrentState, sizeof(m_CurrentState));

			if(FAILED(hr = m_InputDevice->GetDeviceState(sizeof(m_CurrentState), &m_CurrentState)))
			{
				if(SUCCEEDED(m_InputDevice->Acquire()))
				{
					m_InputDevice->GetDeviceState(sizeof(m_CurrentState), &m_CurrentState);
				}
			}

			RECT rect{};
			POINT point{};
			GetWindowRect(m_Application->getWindowHandle(), &rect);
			GetCursorPos(&point);
			if(Utility::isMouseHovering(&rect, &point))
			{
				m_X = point.x - rect.left;
				m_Y = point.y - rect.top;
			}
			m_Scroll = m_CurrentState.lZ;
		}
	}



	
}
