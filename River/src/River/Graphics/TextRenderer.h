#pragma once

#include "Window.h"
#include "ImageRenderer.h"
#include "Text/Font.h"
#include "Alignment.h"

namespace River {
	
	// TODO: Remove this class -> It's temporary! (also why it's defined in header)
	class TextRenderer : public ImageRenderer {

	
	public:
		TextRenderer(Window* window) : ImageRenderer(window) {
			
		}

		void drawText(const std::string& text, Font* font, const Color& color, float x, float y, float z, Alignment alignment = Align::CENTER_LEFT) {

			//Font::TextSize textSize = font->calculateTextSize(text);
			
			for( const char& c : text ) {
				const Font::Glyph& glyph = font->getGlyph(c);
				float glyphX = x + glyph.bearingX + glyph.sprite->getWidth() /2.0;
				float glyphY = y + glyph.bearingY - glyph.sprite->getHeight() /2.0;
				x += glyph.advance;

				drawSprite(glyph.sprite, { glyphX, glyphY, z, (float) glyph.sprite->getWidth(), (float) glyph.sprite->getHeight(), 0, color });
			}

		}


	private:
		using ImageRenderer::drawSprite;
	};
}



