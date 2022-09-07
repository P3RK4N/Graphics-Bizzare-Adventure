#include "pch.h"
#include "FPSComponent.h"

#include "Engine/Core/ApplicationTime.h"

namespace Engine
{
	RTTI_DEFINITIONS(FPSComponent)

	void FPSComponent::update(const ApplicationTime& applicationTime)
	{
		if(applicationTime.getTotalApplicationTime() - m_LastTotalElapsedTime >= 1)
		{
			m_LastTotalElapsedTime = applicationTime.getTotalApplicationTime();
			m_FrameRate = m_FrameCount;
			m_FrameCount = 0;
		}
		m_FrameCount += 1;
	}

	void FPSComponent::draw(const ApplicationTime& applicationTime)
	{
		m_SpriteBatch->Begin();

		std::wostringstream fpsLabel;
		fpsLabel << std::setprecision(4) << L"Frame Rate: " << m_FrameRate << "\nTotal Elapsed Time: " << applicationTime.getTotalApplicationTime();
		m_SpriteFont->DrawString(m_SpriteBatch, fpsLabel.str().c_str(), m_TextPosition);

		m_SpriteBatch->End();
	}
}