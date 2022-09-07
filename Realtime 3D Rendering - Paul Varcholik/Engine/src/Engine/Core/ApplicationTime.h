#pragma once

namespace Engine
{
	class ApplicationTime
	{
	public:
		ApplicationTime() : m_ElapsedApplicationTime(0.0), m_TotalApplicationTime(0.0) {}

		double getElapsedApplicationTime() const { return m_ElapsedApplicationTime; }
		double getTotalApplicationTime() const { return m_TotalApplicationTime; }
		void setElapsedApplicationTime(double elapsedApplicationTime) { m_ElapsedApplicationTime = elapsedApplicationTime; }
		void setTotalApplicationTime(double totalApplicationTime) { m_TotalApplicationTime = totalApplicationTime; }

	private:
		double m_ElapsedApplicationTime;
		double m_TotalApplicationTime;
	};
}