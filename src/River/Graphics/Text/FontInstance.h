#pragma once

#include <string>
#include <unordered_map>

#include "River/Graphics/Texture/Sprite.h"

namespace River {

	class FontInstance {
		friend class Font;
	
	public:
		struct Glyph {
			Texture* const texture;
			const int bearingX, bearingY;
			const int advance;
			const int yMax, yMin;

			Glyph(Texture* texture, int bearingX, int bearingY, int advance, int yMax, int yMin)
				: texture(texture), bearingX(bearingX), bearingY(bearingY), advance(advance), yMax(yMax), yMin(yMin) {
			}
		};	


	public:

		const Glyph &getGlyph(unsigned int characterValue); 

		struct TextSize { unsigned int width = 0, height = 0; };
		TextSize calculateTextSize(const std::string& text);


		/**
		 * @brief Returns the Font's Height. Note: some glyphs may go beyond this height
		*/
		unsigned int getHeight();
	
	
	private:
		FontInstance(unsigned int size, void* nativeFontType);
		~FontInstance();

		FontInstance(const FontInstance& other) = delete;
		FontInstance& operator=(const FontInstance&) = delete;
		
		const Glyph &createGlyph(unsigned int characterValue);


	private:
		const void* nativeFontType;
		const unsigned int size;
		unsigned int height;
		std::unordered_map<unsigned int, Glyph> glyphMap;



	};

}


