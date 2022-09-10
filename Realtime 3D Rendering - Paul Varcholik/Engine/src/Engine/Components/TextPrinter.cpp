#include "pch.h"
#include "TextPrinter.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/RenderStateHelper.h"

namespace Engine
{
	RTTI_DEFINITIONS(TextPrinter)

	void Engine::TextPrinter::draw(const ApplicationTime& applicationTime)
	{
		RenderStateHelper helper(m_Application);
		helper.saveAll();

		m_SpriteBatch->Begin();
		m_SpriteFont->DrawString(m_SpriteBatch, m_Text, m_TextPosition);
		m_SpriteBatch->End();

		helper.restoreAll();
	}
}
