#include "pch.h"

#include "ApplicationClock.h"
#include "ApplicationTime.h"

namespace Engine
{
	ApplicationClock::ApplicationClock()
		: m_CurrentTime(), m_StartTime(), m_LastTime(), m_Frequency()
	{
		m_Frequency = getFrequency();
		reset();
	}

	void ApplicationClock::reset()
	{
		getTime(m_StartTime);
		m_CurrentTime = m_StartTime;
		m_LastTime = m_CurrentTime;
	}

	double ApplicationClock::getFrequency() const
	{
		LARGE_INTEGER frequency;
		ASSERT(QueryPerformanceFrequency(&frequency));
		return (double)frequency.QuadPart;
	}

	void ApplicationClock::updateApplicationTime(ApplicationTime& applicationTime)
	{
		getTime(m_CurrentTime);
		applicationTime.setTotalApplicationTime((m_CurrentTime.QuadPart - m_StartTime.QuadPart) / m_Frequency);
		applicationTime.setElapsedApplicationTime((m_CurrentTime.QuadPart - m_LastTime.QuadPart) / m_Frequency);
		m_LastTime = m_CurrentTime;
	}
}