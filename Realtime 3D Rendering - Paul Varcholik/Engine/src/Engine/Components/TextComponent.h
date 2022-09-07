#pragma once

#include "DrawableApplicationComponent.h"

#include <sstream>
#include <iomanip>

#include <SpriteBatch.h>
#include <SpriteFont.h>

class DirectX::SpriteBatch;
class DirectX::SpriteFont;

namespace Engine
{
	class TextComponent : public DrawableApplicationComponent
	{
		RTTI_DECLARATIONS(TextComponent, DrawableApplicationComponent)

	public:
		TextComponent(Application* app) 
			: DrawableApplicationComponent(app) {}
		TextComponent(Application* app, DirectX::XMFLOAT2 textPosition)
			: DrawableApplicationComponent(app), m_TextPosition(textPosition) {}
		virtual ~TextComponent() override;

		virtual void initialize() override;
		
		DirectX::XMFLOAT2& textPosition() { return m_TextPosition; }

	protected:
		TextComponent() = default;
		TextComponent(const TextComponent&) = default;
		TextComponent& operator=(const TextComponent&) = default;

		DirectX::SpriteBatch* m_SpriteBatch = nullptr;
		DirectX::SpriteFont* m_SpriteFont = nullptr;

		DirectX::XMFLOAT2 m_TextPosition = { 0.0f, 0.0f };

	};
}