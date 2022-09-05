#pragma once

#include "DrawableApplicationComponent.h"

#include <SpriteBatch.h>
#include <SpriteFont.h>

class DirectX::SpriteBatch;
class DirectX::SpriteFont;

namespace Engine
{
	class FPSComponent : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(FPSComponent, DrawableApplicationComponent)

	public:
		FPSComponent(Application& app) 
			: DrawableApplicationComponent(app) {}
		virtual ~FPSComponent() override;

		DirectX::XMFLOAT2& textPosition() { return m_TextPosition; }
		int getFrameRate() const { return m_FrameRate; }

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;

	private:
		FPSComponent() = default;
		FPSComponent(const FPSComponent&) = default;
		FPSComponent& operator=(const FPSComponent&) = default;

		DirectX::SpriteBatch* m_SpriteBatch = nullptr;
		DirectX::SpriteFont* m_SpriteFont = nullptr;

		DirectX::XMFLOAT2 m_TextPosition = { 0.0f, 0.0f };

		int m_FrameRate = 0;
		int m_FrameCount = 0;
		double m_LastTotalElapsedTime = 0;
	};
}