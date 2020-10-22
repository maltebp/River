#include "FontType.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace River {

	FontType::FontType(const std::string& name, const std::string& path, void *nativeLibrary) :
		name(name)
	{
		std::string fullPath = path + "/" + name + ".ttf";
		if( FT_New_Face( (FT_Library) nativeLibrary, fullPath.c_str(), 0, (FT_Face*) &nativeFontType) ) {
			// TODO: Implement proper exception
			throw River::AssetException("Error when loading font '" + path + "'");
		}
	}


	Font* FontType::getFont(unsigned int size) {
		auto it = fontMap.find(size);
		if( it != fontMap.end() )
			return it->second;
		return createFont(size);
	}


	Font* FontType::createFont(unsigned int size) {
		auto result = fontMap.emplace(size, new Font(size, nativeFontType));
		return result.first->second; // Not sure if this actually returns the reference??
	}


	const std::string& FontType::getName() const {
		return name;
	}
}