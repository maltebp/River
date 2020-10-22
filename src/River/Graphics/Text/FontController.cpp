#include "FontController.h"

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "River/Error.h"


namespace River {


	bool FontController::initialized = false;
	void* FontController::nativeLibrary = 0;

	std::string FontController::fontFolder = "";

	std::unordered_map<std::string, FontType*> FontController::fontTypeMap;


	void FontController::initialize() {
		if( initialized ) return;

		if( FT_Init_FreeType((FT_Library*) &nativeLibrary) ) {
			// TODO: Proper exception here
			throw River::Exception("Could not initialize FreeType library");
		}

		initialized = true;
	}


	void FontController::setFontFolder(const std::string &fontFolder) {
		FontController::fontFolder = fontFolder;
	}


	Font* FontController::getFont(const std::string &fontName, unsigned int size) {
		if( !initialized ) initialize();

		// Get font type
		auto it = fontTypeMap.find(fontName);
		if( it == fontTypeMap.end() )
			it = fontTypeMap.emplace(fontName, new FontType(fontName, fontFolder, nativeLibrary)).first;
		return it->second->getFont(size);
	}

	
	
}