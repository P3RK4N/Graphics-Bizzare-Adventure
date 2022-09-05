#pragma once

#include <Windows.h>
#include <exception>

namespace Engine
{
	class ApplicationTime;

	class ApplicationClock
	{
	public:
		ApplicationClock();

		const LARGE_INTEGER& startTime() const { return m_StartTime; }
		const LARGE_INTEGER& currentTime() const { return m_CurrentTime; }
		const LARGE_INTEGER& lastTime() const { return m_LastTime; }

		void reset();
		double getFrequency() const;
		void getTime(LARGE_INTEGER& time) const { QueryPerformanceCounter(&time); }
		void updateApplicationTime(ApplicationTime& applicationTime);

	private:
		ApplicationClock(const ApplicationClock& rhs);
		ApplicationClock& operator=(const ApplicationClock& rhs);

		LARGE_INTEGER m_StartTime;
		LARGE_INTEGER m_CurrentTime;
		LARGE_INTEGER m_LastTime;
		double m_Frequency;
	};
}