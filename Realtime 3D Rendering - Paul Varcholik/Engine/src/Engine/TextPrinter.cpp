#include "pch.h"
#include "TextPrinter.h"

namespace Engine
{
	RTTI_DEFINITIONS(TextPrinter)

	void Engine::TextPrinter::draw(const ApplicationTime& applicationTime)
	{
		m_SpriteBatch->Begin();

		m_SpriteFont->DrawString(m_SpriteBatch, m_Text, m_TextPosition);

		m_SpriteBatch->End();
	}
}
