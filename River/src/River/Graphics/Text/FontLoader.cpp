#include "FontLoader.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "River/Error.h"


namespace River {


    void FontLoader::init() {
       /* if( FT_Init_FreeType(&freetype) ) {
            throw River::Exception("Could not initialize FreeType library");
        }*/
    }


    void FontLoader::loadFont(const std::string &path) {
   /*     FT_Face face;
        if( FT_New_Face(freetype, path.c_str(), 0, &face) ) {
            throw River::AssetException("Error when loading font '" + path + "'");
        }*/

        //face->glyph->bitmap.buffer
    }


    

    void convertToRGB(unsigned int width, unsigned int height, unsigned int* buffer) {
        


    }


    Texture* FontLoader::loadGlyph(const std::string &path, char glyph) {
        FT_Library freetype;
        if( FT_Init_FreeType(&freetype) ) {
            throw River::Exception("Could not initialize FreeType library");
        }

        FT_Face face;
        if( FT_New_Face(freetype, path.c_str(), 0, &face) ) {
            throw River::AssetException("Error when loading font '" + path + "'");
        }

        FT_Set_Pixel_Sizes(face, 0, 400);

        // load character
        int errCode = 0;
        if( (errCode = FT_Load_Char(face, glyph, FT_LOAD_RENDER)) ) {
            std::cout << "Error code: " << errCode << std::endl;
            throw River::AssetException("Error when loading font glyph '" + path + "'");
        }

        auto &image = face->glyph->bitmap;
        Texture *glyphTexture = new Texture(image.width, image.rows, 1, 1, image.buffer);

        return glyphTexture;
    }



}