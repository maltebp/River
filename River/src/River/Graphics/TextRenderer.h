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
			//enableBlending();
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
				
				float glyphX = x + glyph.texture->getWidth() /2.0;
				if( first ) first = false;
				else glyphX += glyph.bearingX;
				
				float glyphY = y + glyph.bearingY - glyph.texture->getHeight() /2.0;
				x += glyph.advance;

				drawSprite(
					glyph.texture,
					{
						glyphX, glyphY, z,
						(float)glyph.texture->getWidth(), (float)glyph.texture->getHeight(),
						0, 
						color.a, { color.r, color.g, color.b, 1.0 }
					}
				);
			}

		}


	private:
		using ImageRenderer::drawSprite;
	};
}



