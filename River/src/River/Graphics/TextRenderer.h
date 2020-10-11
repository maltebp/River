#pragma once

#include "Window.h"
#include "ImageRenderer.h"
#include "Text/Font.h"
#include "River/Primitives/Alignment.h"

namespace River {
	
	// TODO: Remove this class -> It's temporary! (also why it's defined in header)
	class TextRenderer : public ImageRenderer {

	
	public:
		TextRenderer(Window* window) : ImageRenderer(window) {
			
		}

		void drawText(const std::string& text, Font* font, const Color& color, float x, float y, float z, Alignment alignment = Align::CENTER_LEFT) {

			Font::TextSize textSize = font->calculateTextSize(text);

			if( alignment.isHCenter() )
				x -= textSize.width / 2.0;

			if( alignment.isRight() )
				x -= textSize.width;

			if( alignment.isVCenter() )
				y -= font->getHeight() / 4;

			if( alignment.isTop() )
				y -= font->getHeight() / 1.5;

			
			bool first = true;
			for( const char& c : text ) {
				const Font::Glyph& glyph = font->getGlyph(c);
				
				float glyphX = x + glyph.sprite->getWidth() /2.0;
				if( first ) first = false;
				else glyphX += glyph.bearingX;
				
				float glyphY = y + glyph.bearingY - glyph.sprite->getHeight() /2.0;
				x += glyph.advance;

				drawSprite(glyph.sprite, { glyphX, glyphY, z, (float) glyph.sprite->getWidth(), (float) glyph.sprite->getHeight(), 0, color });
			}

		}


	private:
		using ImageRenderer::drawSprite;
	};
}



