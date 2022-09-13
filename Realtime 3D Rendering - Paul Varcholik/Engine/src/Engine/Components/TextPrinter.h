#pragma once
#include "TextComponent.h"

namespace Engine
{
	class TextPrinter : public TextComponent
	{
		RTTI_DECLARATIONS(TextPrinter, TextComponent)

	public:
		TextPrinter(Application* app) 
			: TextComponent(app) {}
		TextPrinter(Application* app, DirectX::XMFLOAT2 textPosition)
			: TextComponent(app, textPosition) {}

		virtual void draw(const ApplicationTime& applicationTime) override;

		void setText(std::string text) { memset(m_Text, 0, sizeof(m_Text));  strcpy_s(m_Text, text.length(), text.c_str()); }

	protected:
		static const unsigned s_TextSize = 512;
		char m_Text[s_TextSize]{};
	};
}
