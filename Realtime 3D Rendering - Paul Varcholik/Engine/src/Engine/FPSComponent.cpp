#include "pch.h"
#include "FPSComponent.h"
#include "Application.h"

#include <sstream>
#include <iomanip>
#include <utility>

namespace Engine
{
	RTTI_DEFINITIONS(FPSComponent)

	FPSComponent::~FPSComponent()
	{
		DeleteObject(m_SpriteFont);
		DeleteObject(m_SpriteBatch);
	}

	void FPSComponent::initialize()
	{
		m_SpriteBatch = new DirectX::SpriteBatch(m_Application->getDirect3DDeviceContext());
		m_SpriteFont = new DirectX::SpriteFont(m_Application->getDirect3DDevice(), L"resources/comicSansMS16.spritefont");
	}

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