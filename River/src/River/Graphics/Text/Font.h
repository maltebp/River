#pragma once

#include <string>
#include <unordered_map>

#include "River/Graphics/Texture/Sprite.h"

namespace River {

	class Font {
		struct Glyph {
			const Sprite * const sprite;
			const int bearingX, bearingY;
			const int advance;

			Glyph(Sprite* sprite, int bearingX, int bearingY, int advance)
			:	sprite(sprite), bearingX(bearingX), bearingY(bearingY), advance(advance) { }
		};

		const void *nativeFontType;
		const unsigned int size;
		std::unordered_map<unsigned int, Glyph> glyphMap;


	public:
		Font(unsigned int size, void* nativeFontType);

		const Glyph &getGlyph(const std::string &character); 

		unsigned int calculateLength(const std::string &text);
	
	
	private:
		const Glyph &createGlyph(unsigned int characterValue);

		Font(const Font &other);
		Font &operator=(const Font &);
	
	};

}


