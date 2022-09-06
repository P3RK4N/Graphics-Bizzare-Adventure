#pragma once
#include "TextComponent.h"

namespace Engine
{
	class FPSComponent : public TextComponent
	{
		RTTI_DECLARATIONS(FPSComponent, TextComponent)

	public:
		FPSComponent(Application* app) 
			: TextComponent(app) {}
		FPSComponent(Application* app, DirectX::XMFLOAT2 textPosition)
			: TextComponent(app, textPosition) {}

		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;

		int getFrameRate() const { return m_FrameRate; }

	protected:
		int m_FrameRate = 0;
		int m_FrameCount = 0;
		double m_LastTotalElapsedTime = 0;
	};
}