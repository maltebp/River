#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "River/Error.h"

namespace River{

	Font::Font(unsigned int size, void* nativeFontType) :
		size(size), nativeFontType(nativeFontType)
	{   

        FT_Face face = (FT_Face)nativeFontType; 
        FT_Set_Pixel_Sizes(face, 0, size);
        height = size;//  (face->size->metrics.ascender >> 6) - (face->size->metrics.descender >> 6); // face->size->metrics.height >> 6;

        glyphMap.reserve(sizeof(Glyph)*93);
        // Range is all printable ASCII characters
        for( unsigned int  characterValue = 33;  characterValue < 126;  characterValue++ ) {
            createGlyph(characterValue);
        }
	}


	const Font::Glyph& Font::getGlyph(unsigned int characterValue) {    
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
                new Texture(new Image(texture.width, texture.rows, 1, 1, texture.buffer)),
                face->glyph->bitmap_left, face->glyph->bitmap_top,
                face->glyph->advance.x >> 6,
                face->glyph->bitmap_top,  face->glyph->bitmap_top-((int)texture.rows)
            }
        );

        return it.first->second;
    }


    Font::TextSize Font::calculateTextSize(const std::string &text) {
        unsigned int minY = 0, maxY = 0, length = 0, glyphMinY, glyphMaxY;

        TextSize size;
        for( const char& c : text ) {
            if( c < 0 ) throw new River::NotImplementedException("Font::calculateTextLength: Non-ASCII characters are not supported yet");
            if( c < 32 ) throw new River::Exception("Control characters cannot be rendered");
            
            const Glyph& glyph = getGlyph(c);
            length += glyph.advance;

            if( glyph.yMax > maxY ) maxY = glyph.yMax;
            if( glyph.yMin < minY ) minY = glyph.yMin;
        }
        if( minY < 0 ) minY *= -1;
        return TextSize{ length, maxY + minY };
	}

    unsigned int Font::getHeight() {
        return height;
    }
}