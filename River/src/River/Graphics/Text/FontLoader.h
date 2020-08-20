#pragma once

#include <string>


#include "River/Graphics/Texture/Texture.h"

namespace River {


    class FontLoader {



        static void init();


    public:

        static void loadFont(const std::string &path);
        static Texture* loadGlyph(const std::string &path, char glyph);

    };

    


}


