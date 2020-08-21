#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "River/Error.h"

namespace River{

	Font::Font(unsigned int size, void* nativeFontType) :
		size(size), nativeFontType(nativeFontType)
	{   
        glyphMap.reserve(sizeof(Glyph)*93);
        // Range is all printable ASCII characters
        for( unsigned int  characterValue = 33;  characterValue < 126;  characterValue++ ) {
            createGlyph(characterValue);
        }
	}


	const Font::Glyph& Font::getGlyph(const std::string &character) {
        // TODO: Create conversion from unicode codepoints to value
        unsigned int characterValue = character[0];
        
        auto it = glyphMap.find(characterValue);
        if( it != glyphMap.end() ) return it->second;
        return createGlyph(characterValue);
	}


    const Font::Glyph& Font::createGlyph(unsigned int characterValue) {
        FT_Face face = (FT_Face) nativeFontType;

        FT_Set_Pixel_Sizes(face, 0, size);

        // TODO: Proper error handling
        int loadResult = FT_Load_Char(face, characterValue, FT_LOAD_RENDER);
        if( loadResult != 0 ) {
            std::string msg = "Could not load glyph "; msg += ((unsigned char)characterValue);  msg += " for font";
            throw new River::AssetException(msg);
            // Should it just throw warning instead? Does it make sense to throw exception if it can't load glyph?
        }

        auto &texture = face->glyph->bitmap;
        auto it = glyphMap.emplace(
            characterValue,
            Glyph{
                new Sprite(new Texture(texture.width, texture.rows, 1, 1, texture.buffer)),
                face->glyph->bitmap_left, face->glyph->bitmap_top,
                face->glyph->advance.x
            }
        );

        return it.first->second;
    }


	unsigned int Font::calculateLength(const std::string &text) {
        throw new River::NotImplementedException("Font::calculateLength has not been implemented yet");
	}
}