#include "pch.h"
#include "TextComponent.h"
#include "Application.h"

namespace Engine
{
	RTTI_DEFINITIONS(TextComponent)

	TextComponent::~TextComponent()
	{
		DeleteObject(m_SpriteFont);
		DeleteObject(m_SpriteBatch);
	}

	void TextComponent::initialize()
	{
		m_SpriteBatch = new DirectX::SpriteBatch(m_Application->getDirect3DDeviceContext());
		m_SpriteFont = new DirectX::SpriteFont(m_Application->getDirect3DDevice(), L"resources/comicSansMS16.spritefont");
	}

}